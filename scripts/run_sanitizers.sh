./"${ZIP_NAME}"/"${EXECUTABLE_NAME}" &
sleep 1 && xdotool search --name "${APP_WINDOW}" key Down &&
sleep 1 && xdotool search --name "${APP_WINDOW}" key Down &&
sleep 1 && xdotool search --name "${APP_WINDOW}" key Return &&
sleep 1 && xdotool search --name "${APP_WINDOW}" keydown W &&
sleep 10 && xdotool search --name "${APP_WINDOW}" key Escape &&
sleep 1 && xdotool search --name "${APP_WINDOW}" key Escape

