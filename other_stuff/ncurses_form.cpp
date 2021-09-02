#include <algorithm>
#include <fcntl.h>
#include <form.h>
#include <json.hpp>
#include <string>
#include <unistd.h>

#define FILENAME "some_configs.json"
#define FILE_MAX_SIZE 0xffff

int main()
{
    // Opening json file, reading and parsing it
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0)
        return fd;
    char buf[FILE_MAX_SIZE];
    ssize_t len = read(fd, buf, sizeof(buf));
    close(fd);
    if (len < 0)
        return len;
    nlohmann::json js = nlohmann::json::parse(buf);

    std::vector<std::string> form_index_to_json;

    // Initialize curses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    FIELD *field[js.size()];
    FORM *my_form;
    int ch;

    // Get configurable options from json
    for (auto& element : js.items())
    {
        form_index_to_json.push_back(element.key());

        // Params: new_field(height, width, pos_y, pos_x, offscreen, nbuf)
        // No need for any specific offscreen or buffer settings
        field[form_index_to_json.size() - 1] = new_field(1, 20, form_index_to_json.size() - 1, 58, 0, 0);
        field_opts_off(field[form_index_to_json.size() - 1], O_AUTOSKIP);
        strcpy(buf, element.value().dump().c_str());
        set_field_buffer(field[form_index_to_json.size() - 1], 0, buf);
        if (element.value().type() == nlohmann::json::value_t::number_integer ||
            element.value().type() == nlohmann::json::value_t::number_unsigned ||
            element.value().type() == nlohmann::json::value_t::number_float)
            set_field_type(field[form_index_to_json.size() - 1], TYPE_NUMERIC);
    }

    // Create form and print it
    field[js.size()] = NULL;
    my_form = new_form(field);
    post_form(my_form);
    refresh(); // The fields
    for (uint8_t i = 0; i < form_index_to_json.size() && i < LINES; ++i)
        mvprintw(i, 0, js[form_index_to_json[i]].dump().c_str());
    mvprintw(LINES - 2, 0, "Exit with F1");
    refresh(); // The field descriptions (keys)

    // Main input reading loop
    while ((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
        case 10: // Enter
        case KEY_DOWN:
            form_driver(my_form, REQ_END_LINE);
            form_driver(my_form, REQ_NEXT_FIELD);
            form_driver(my_form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(my_form, REQ_END_LINE);
            form_driver(my_form, REQ_PREV_FIELD);
            form_driver(my_form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(my_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(my_form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
            form_driver(my_form, REQ_DEL_PREV);
            break;
        default:
            form_driver(my_form, ch);
            break;
        }
    }

    // Get data from form and write it to json in correct format
    for (uint8_t i = 0; i < form_index_to_json.size(); ++i)
    {
        if (js[form_index_to_json[i]].type() == nlohmann::json::value_t::number_integer ||
            js[form_index_to_json[i]].type() == nlohmann::json::value_t::number_unsigned)
        {
            int ib = std::stoi(field_buffer(field[i], 0));
            js[form_index_to_json[i]] = ib;
        }
        else if (js[form_index_to_json[i]].type() == nlohmann::json::value_t::number_float)
        {
            float f = std::stof(field_buffer(field[i], 0));
            js[form_index_to_json[i]] = f;
        }
        else if (js[form_index_to_json[i]].type() == nlohmann::json::value_t::array)
        {
            strcpy(buf, field_buffer(field[i], 0));
            js[form_index_to_json[i]] = nlohmann::json::parse(buf);
        }
        else
        {
            std::string str(field_buffer(field[i], 0));

            // Remove extra quotes and empty spaces
            str.erase(std::remove_if(str.begin(), str.end(),
                                     [](uint8_t x) { return (std::isspace(x) || x == '"'); }),
                      str.end());

            js[form_index_to_json[i]] = str;
        }
    }

    // Write json file
    fd = open(FILENAME, O_WRONLY | O_TRUNC);
    if (fd < 0)
        return fd;
    len = write(fd, js.dump().c_str(), js.dump().size());
    len = write(fd, "\0", 1); // End the string
    close(fd);
    if (len < 0)
        return len;

    mvprintw(LINES - 2, 0, "Changes succesfully saved to file " FILENAME "! (Press any key to exit)");
    refresh();
    getch();

    // Un post form and free the memory
    unpost_form(my_form);
    free_form(my_form);
    for (uint8_t i = 0; i < form_index_to_json.size(); ++i)
        free_field(field[i]);
    endwin();

    return 0;
}