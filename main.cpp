#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/sound.hpp>
#include <mousetrap/include/application.hpp>
#include <mousetrap/include/window.hpp>
#include <mousetrap/include/motion_event_controller.hpp>
#include <mousetrap/include/label.hpp>
#include <mousetrap/include/text_view.hpp>
#include <mousetrap/include/box.hpp>
#include <mousetrap/include/frame.hpp>
#include <mousetrap/include/button.hpp>
#include <mousetrap/include/list_view.hpp>
#include <mousetrap/include/column_view.hpp>
#include <mousetrap/include/popover_menu_button.hpp>
#include <mousetrap/include/stack.hpp>
#include <mousetrap/include/notebook.hpp>
#include <mousetrap/include/spin_button.hpp>
#include <mousetrap/include/drop_down.hpp>
#include <mousetrap/include/scale.hpp>
#include <mousetrap/include/file_system.hpp>
#include <mousetrap/include/spinner.hpp>
#include <mousetrap/include/grid_view.hpp>
#include <mousetrap/include/image_display.hpp>
#include <mousetrap/include/scrolled_window.hpp>
#include <mousetrap/include/icon.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

inline Window* window = nullptr;
inline Application* app = nullptr;

inline Action* action;

static void startup(GApplication*)
{
    window = new Window(*app);
    window->set_show_menubar(true);

    action = new Action("global.test_action");
    action->set_function([](auto test2){
    }, "testest");
    action->add_shortcut("<Control>c");
    app->add_action(action);

    auto menu_model = MenuModel();
    menu_model.add_action("test", *action);

    window->connect_signal_close_request([](Window*) -> bool {
        std::cout << "close" << std::endl;
        return false;
    });

    auto* box = new Box(Orientation::VERTICAL);
    auto* spinner = new Spinner();

    auto* button_spin = new Button();

    auto* theme = gtk_icon_theme_new();
    gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_resource_path(theme, "/home/clem/Desktop/");
    gtk_icon_theme_add_search_path(theme, "/home/clem/Desktop/");

    auto* view = new ListView(Orientation::VERTICAL);

    char** list = gtk_icon_theme_get_icon_names(theme);
    size_t i = 0;
    while (list[i] != nullptr)
    {
        auto* paintable = gtk_icon_theme_lookup_icon(theme, list[i], nullptr, 64, 1, GTK_TEXT_DIR_NONE, GTK_ICON_LOOKUP_FORCE_REGULAR);
        auto* image = new ImageDisplay(GTK_IMAGE(gtk_image_new_from_paintable(GDK_PAINTABLE(paintable))));
        auto* label = new Label(list[i]);
        auto* box = new Box(Orientation::HORIZONTAL);

        image->set_margin_end(10);
        box->push_back(image);
        box->push_back(label);

        view->push_back(box);
        i += 1;
    }

    auto* scrolled_window = new ScrolledWindow();
    scrolled_window->set_child(view);

    //window->set_child(scrolled_window);

    auto* icon = new Icon();
    icon->create_from_file("/home/clem/Desktop/icons/hicolor/48x48/rat_icon_desktop.svg");

    auto* image_display = new ImageDisplay();
    image_display->create_from_icon(*icon);

    window->set_child(image_display);

    //window->set_child(box);
    window->show();
    window->present();
    window->set_is_focusable(true);
    window->grab_focus();
}

int main()
{
    app = new Application("app.mousetrap");
    app->connect_signal_activate([](Application* app)
    {
        auto* model = new MenuModel();
        auto* submenu = new MenuModel();
        auto* action = new Action("test");
        action->set_function([](){
            std::cout << "test" << std::endl;
        });
        app->add_action(action);

        submenu->add_action("test", *action);
        //model->add_submenu("test", submenu);
        app->set_menubar(model);

        auto* popover_menu = new PopoverMenu(submenu);
        auto* button = new PopoverMenuButton();
        button->set_popover_menu(popover_menu);
        //window->set_child(button);

        gtk_application_set_menubar(app->operator GtkApplication *(), model->operator GMenuModel *());
    });

    app->connect_signal("startup", startup);

    auto out = app->run();

    delete app;
    delete window;

    return out;
    /*
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(0, 0, 8, 3, 2));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    initialize_opengl();

    std::set<Vector2f> test;

    std::deque<Shape> shapes;
    for (size_t x = 0; x < 4; ++x)
    {
        shapes.emplace_back();
        shapes.back().as_rectangle(Vector2f(x / 4.f), Vector2f{0.5, 0.5});
        shapes.back().set_color(HSVA(rand() / float(RAND_MAX), 1, 1, 1));
    }

    std::deque<RenderTask> tasks;
    for (auto& shape : shapes)
    {
        tasks.emplace_back(&shape);
    }

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& task : tasks)
            task.render();

        glFlush();
        window.display();
    }

    return 0;
     */
}