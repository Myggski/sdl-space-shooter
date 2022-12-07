#include "pch.h"
#define SDL_MAIN_HANDLED

#include "application/application.h"
#include "application/application_props.h"

int main(int argc, char* args[])
{
    application::application app = application::application(application::application_props("SDL Space Shooter", 800, 600));

    app.init();
    app.run_game();

    return 0;
}
