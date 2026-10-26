#include "textprompt.h"

#include <utility>

namespace
{
	constexpr size_t kMaxLength = 32;
	constexpr int kBarHeight = 20;
}

void TextPrompt::AskText(std::string newLabel, TextCallback submit, CancelCallback cancel)
{
	label = std::move(newLabel);
	text.clear();
	onSubmit = std::move(submit);
	onCancel = std::move(cancel);
	active = true;
}

void TextPrompt::AskInt(std::string newLabel, IntCallback submit, CancelCallback cancel)
{
	AskText(newLabel, [this, newLabel, submit, cancel](const std::string& s)
	{
		try
		{
			int value = std::stoi(s);
			if (value > 0)
			{
				submit(value);
				return;
			}
		}
		catch (...)
		{
			// not a number, ask again below
		}
		AskInt(newLabel, submit, cancel);
	}, cancel);
}

void TextPrompt::Close()
{
	active = false;
	text.clear();
}

void TextPrompt::Update(olc::PixelGameEngine& g)
{
	if (!active)
		return;

	if (g.GetKey(olc::ESCAPE).bPressed)
	{
		auto cancel = std::move(onCancel);
		Close();
		if (cancel)
			cancel();
		return;
	}

	if (g.GetKey(olc::ENTER).bPressed || g.GetKey(olc::RETURN).bPressed)
	{
		auto submit = std::move(onSubmit);
		std::string result = text;
		Close();
		if (submit)
			submit(result);
		return;
	}

	if (g.GetKey(olc::BACK).bPressed && !text.empty())
		text.pop_back();

	if (text.size() >= kMaxLength)
		return;

	// Letters: shift for capitals. Digits: top row or numpad. Numpad minus for '-'.
	bool shift = g.GetKey(olc::SHIFT).bHeld;
	for (int k = olc::A; k <= olc::Z; k++)
	{
		if (g.GetKey(static_cast<olc::Key>(k)).bPressed)
			text += char((shift ? 'A' : 'a') + (k - olc::A));
	}
	for (int d = 0; d <= 9; d++)
	{
		if (g.GetKey(static_cast<olc::Key>(olc::K0 + d)).bPressed || g.GetKey(static_cast<olc::Key>(olc::NP0 + d)).bPressed)
			text += char('0' + d);
	}
	if (g.GetKey(olc::NP_SUB).bPressed)
		text += '-';
}

void TextPrompt::Draw(olc::PixelGameEngine& g)
{
	if (!active)
		return;

	// Other code can leave the draw target or pixel mode changed; the bar must land on the main screen
	g.SetDrawTarget(nullptr);
	g.SetPixelMode(olc::Pixel::NORMAL);

	int y = g.ScreenHeight() - kBarHeight;
	g.FillRect(olc::vi2d(0, y), olc::vi2d(g.ScreenWidth(), kBarHeight), olc::BLACK);
	g.DrawStringDecal(olc::vf2d(4.0f, float(y + 6)), label + " " + text + "_", olc::WHITE);
}
