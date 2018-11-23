#pragma once

struct loginfo {

	loginfo(std::string text, ImColor color, float time) {
		this->text = text;
		this->color = color;
		this->time = time;
	}

	std::string text;
	ImColor color;
	float time;

};

namespace EventLogger {

	static std::deque<loginfo> events = {};

	void AddEvent(std::string text, ImColor color = ImColor(255, 255, 255));

	// Hooks
	void PaintImGui();

	void FireGameEvent(IGameEvent* event);

}
