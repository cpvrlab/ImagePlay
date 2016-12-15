#ifndef FVVERSIONCOMPARATOR_H
#define FVVERSIONCOMPARATOR_H

#include <iosfwd>
#include <vector>


class FvVersionComparator
{
public:

	typedef enum {
		kSame = 0,
		kDescending = 1,
		kAscending = -1
	} ComparatorResult;

	static ComparatorResult CompareVersions(std::string versionA,
											std::string versionB);

private:

	FvVersionComparator();

	typedef enum {
		kNumberType,
		kStringType,
		kSeparatorType
	} CharacterType;

	static CharacterType TypeOfCharacter(std::string character);
	static std::vector<std::string> SplitVersionString(std::string version);

};

#endif // FVVERSIONCOMPARATOR_H
