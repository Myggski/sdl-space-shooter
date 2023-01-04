#include "pch.h"
#define SDL_MAIN_HANDLED

#include "application/application.h"
#include "application/application_props.h"

int main(int argc, char* args[])
{
    application::application app{ application::application_props("Bug Shooter", 1280, 720) };

    app.init();
    app.run();

    return 0;
}
