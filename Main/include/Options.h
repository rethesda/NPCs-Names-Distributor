#pragma once
#include "NameDefinition.h"

namespace NND
{
	enum NameStyle : uint8_t
	{
		/// Show a full name with a title.
		kDisplayName,

		/// Show only a name without a title.
		kFullName,

		/// Show short name if available, otherwise fallback to kFullName.
		kShortName,

		/// Show only a title if available, otherwise fallback to kFullName.
		///
		///	This includes both custom Title or default title if allowDefaultTitle = true.
		kTitle
	};

	namespace Options
	{
		namespace General
		{
			inline bool enabled = true;
		}

		namespace Obscurity
		{
			inline bool enabled = true;
			inline bool greetings = false;
			inline Name defaultName = "???";
		}

		namespace NameContext
		{
			inline auto kCrosshair = kDisplayName;
			inline auto kCrosshairMinion = kTitle;

			inline auto kSubtitles = kShortName;
			inline auto kDialogue = kFullName;

			inline auto kInventory = kFullName;

			inline auto kBarter = kShortName;

			inline auto kEnemyHUD = kFullName;

			inline auto kOther = kFullName;
		}

		namespace DisplayName
		{
			constexpr std::array defaultFormats = {
				"[name]"sv,                // 0
				"[name][break][title]"sv,  // 1
				"[name] ([title])"sv,      // 2
				"[name] [[title]]"sv,      // 3
				"[name], [title]"sv,       // 4
				"[name]; [title]"sv,       // 5
				"[name]. [title]"sv,       // 6
				"[name] [title]"sv         // 7
			};

			/// Format string for DisplayName.
			///	Supports placeholders:
			///	- [name]: Substitutes full name
			///	- [title]: Substitutes title
			///	- [break]: Substitutes new line.
			inline std::string_view format = "[name] ([title])"sv;
		}

		namespace Hotkeys
		{
			inline std::string_view generateAll = "RCtrl+RShift+G"sv;
			inline std::string_view generateTarget = "RCtrl+G"sv;
			inline std::string_view toggleObscurity = "RCtrl+O"sv;
			inline std::string_view toggleNames = "RCtrl+N"sv;
			inline std::string_view reloadSettings = "RCtrl+L"sv;

			inline std::string_view fixStuckName = "RCtrl+Backspace"sv;
		}

		void Save();
		void Load();
	}
}