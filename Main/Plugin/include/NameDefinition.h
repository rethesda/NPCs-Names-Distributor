#pragma once
#include "CLIBUtil/Bitmasks.hpp"

namespace NND
{
	using Name = std::string;
	using NameRef = std::string_view;
	using NameIndex = size_t;
	using NamesList = std::vector<Name>;

	constexpr inline NameRef empty = ""sv;

	struct NameComponents
	{
		NameRef firstPrefix = empty;
		NameRef firstName = empty;
		NameRef firstSuffix = empty;

		NameRef middlePrefix = empty;
		NameRef middleName = empty;
		NameRef middleSuffix = empty;

		NameRef lastPrefix = empty;
		NameRef lastName = empty;
		NameRef lastSuffix = empty;

		NameRef conjunction = empty;

		[[nodiscard]] bool IsValid() const {
			return firstName != empty ||
			       middleName != empty ||
			       lastName != empty;
		}

		[[nodiscard]] std::optional<Name> Assemble() const;
	};

	struct NameDefinition
	{
		/// Priority
		enum Priority : uint8_t
		{
			/// Definition with `kRace` priority contains base names specific for races or otherwise a large group that shares an innate trait that cannot be changed.
			/// This is the default one and may be omitted.
			kRace = 0,

			/// Definition with `kClass` priority contains names for a more narrow group that shares an innate trait, which also cannot be changed.
			kClass,

			/// Definition with `kFaction` priority contains names for a medium sized group that are united by some common idea or belief, that are scattered across the world.
			kFaction,

			/// Definition with `kClan` priority contains names for a small group that are united are united by some common idea or belief, but typically located in a single area.
			kClan,

			/// Definition with `kIndividual` priority contains names for distinct individuals that are usually hand-picked.
			kIndividual,

			/// Default priority is `kRace`.
			kDefault = kRace,

			/// The highest possible priority.
			kForced = kIndividual,

			kTotal
		};

		enum class Scope: uint8_t
		{
			kName = 0b1,
			kTitle = 0b10,
			kObscurity = 0b100,

			kDefault = kName,
			kAll = kName | kTitle | kObscurity
		};
		
		struct BaseNamesContainer
		{
			NamesList names{};
			uint8_t   chance = 100;

			/// Checks whether given names container will always produce a name.
			///	Container is considered static when its chance is 100%.
			[[nodiscard]] bool IsStatic() const {
				return chance >= 100;
			}

			/// Checks whether given names container will never produce a name.
			[[nodiscard]] bool IsDisabled() const {
				return chance <= 0 || IsEmpty();
			}

			[[nodiscard]] bool IsEmpty() const {
				return names.empty();
			}

			[[nodiscard]] size_t GetSize() const {
				return names.size();
			}

			[[nodiscard]] std::pair<NameRef, NameIndex> GetRandom(NameIndex maxIndex) const;
			std::pair<NameRef, size_t> GetRandom() const {
				return GetRandom(names.size()-1);
			}
			NameRef GetRandomName(NameIndex maxIndex) const {
				return GetRandom(maxIndex).first;
			}
			NameRef GetRandomName() const {
				return GetRandom().first;
			}

			[[nodiscard]] NameRef GetNameAt(NameIndex index) const {
				return names.at(index);
			}
		};

		struct Adfix : BaseNamesContainer
		{
			/// Flag indicating whether given Adfix can only be applied exclusively.
			///	When one of the exclusive Adfixes is picked the other one will be skipped.
			bool exclusive = false;
		};

		struct NamesVariant : BaseNamesContainer
		{
			Adfix prefix{};
			Adfix suffix{};
		};

		struct NameSegment
		{
			struct Behavior
			{
				/// Flag indicating that NameSegment using this behavior should inherit the same NameSegment
				///	from the next Name Definition if it fails to pick a name from the current one.
				bool shouldInherit = false;

				/// Flag indicating that name part using this behavior should pair prefix and suffix.
				///	This behavior makes Name Definition pick one index that will be used for both prefix and suffix.
				///	If either of prefix or suffix has a non 100% chance then it will determine whether both of them will be used or none.
				///	If both of them has a less than 100% chance, than the prefix's chance will be used.
				bool useCircumfix = false;
			};

			NamesVariant male{};
			NamesVariant female{};
			NamesVariant any{};

			Behavior behavior{};

			/// Checks whether the NameSegment will always produce a name.
			///	NameSegment is considered static when it has at least one not empty NamesVariant and all present NamesVariants are static.
			[[nodiscard]] bool IsStatic() const {
				return male.IsStatic() && female.IsStatic() && any.IsStatic();
			}

			/// NameSegment is considered empty when all its NamesVariants are empty.
			[[nodiscard]] bool IsEmpty() const {
				return male.IsEmpty() && female.IsEmpty() && any.IsEmpty();
			}

			[[nodiscard]] const NamesVariant& GetVariant(RE::SEX sex) const;
		};

		struct Conjunctions
		{
			NamesList male{};
			NamesList female{};
			NamesList any = { " " };

			[[nodiscard]] NameRef          GetRandom(RE::SEX sex) const;
			[[nodiscard]] const NamesList& GetList(RE::SEX sex) const;
		};

		NameSegment firstName{};
		NameSegment middleName{};
		NameSegment lastName{};

		Conjunctions conjunction{};

		/// Name of the definition.
		std::string name;
		Priority    priority = Priority::kDefault;
		Scope scope = Scope::kDefault;

		[[nodiscard]] bool HasDefaultScopes() const {
			return scope == Scope::kDefault;
		}

		void GetRandomFullName(RE::SEX sex, NameComponents& components) const;
		void GetRandomFirstName(RE::SEX sex, NameComponents& components) const;
		void GetRandomMiddleName(RE::SEX sex, NameComponents& components) const;
		void GetRandomLastName(RE::SEX sex, NameComponents& components) const;
		void GetRandomConjunction(RE::SEX sex, NameComponents& components) const;
	};
}

template <>
struct enable_bitmask_operators<NND::NameDefinition::Scope>
{
	static constexpr bool enable = true;
};