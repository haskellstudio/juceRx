#include "Mixer.h"
std::string get_pid() {
	std::stringstream s;
	s << std::this_thread::get_id();
	return s.str();
}

void showmsg(juce::String content, juce::String title, juce::String buttontext)
{
	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, title, content, "ok");
}


std::string oldpid = "";
double g_pos = 0.0;