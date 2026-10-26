#pragma once
#include <functional>
#include <string>
#include "../lib/olcPixelGameEngine.h"

// Single-line text entry drawn over the game (replaces console cin).
// While Active() the game should not process gameplay or editor input.
// Callbacks run after the prompt has closed, so they may open the next prompt.
class TextPrompt
{
public:
	using TextCallback = std::function<void(const std::string&)>;
	using IntCallback = std::function<void(int)>;
	using CancelCallback = std::function<void()>;

	bool Active() const { return active; }

	// Enter -> onSubmit(text). Escape -> onCancel().
	void AskText(std::string label, TextCallback onSubmit, CancelCallback onCancel = nullptr);

	// Like AskText, but only accepts a positive integer and re-asks otherwise.
	void AskInt(std::string label, IntCallback onSubmit, CancelCallback onCancel = nullptr);

	void Update(olc::PixelGameEngine& g);
	void Draw(olc::PixelGameEngine& g);

private:
	void Close();

	bool active = false;
	std::string label;
	std::string text;
	TextCallback onSubmit;
	CancelCallback onCancel;
};
