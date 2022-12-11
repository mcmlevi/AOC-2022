#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <vector>
#include <algorithm>
#include <ranges>
#include <sstream>

struct Instruction
{
	enum class Type
	{
		nop,
		cd,
		ls
	};

	Type						type = Type::nop;
	std::string					instructionContext;
	std::vector<std::string>	instructionResults;
};

Instruction GetNextInstruction(std::ifstream& inFileStream)
{
	Instruction nextInstruction;

	std::string instructionLine;
	std::getline(inFileStream, instructionLine);

	if (instructionLine.find("$ ls") != std::string::npos)
	{
		nextInstruction.type = Instruction::Type::ls;
	}
	else if (instructionLine.find("$ cd") != std::string::npos)
	{
		auto pos = instructionLine.find("cd") + 3;
		nextInstruction.instructionContext = instructionLine.substr(pos, instructionLine.size() - pos);
		nextInstruction.type = Instruction::Type::cd;

		return nextInstruction;
	}

	while (!inFileStream.eof())
	{
		std::string nextLine;
		auto currentPos = inFileStream.tellg();
		std::getline(inFileStream, nextLine);

		if (nextLine.front() == '$')
		{
			inFileStream.seekg(currentPos, std::ios_base::beg);
			break;
		}

		nextInstruction.instructionResults.emplace_back(std::move(nextLine));
	}

	return nextInstruction;
}

struct File
{
	std::string name;
	size_t size;
};

struct Directory
{
	std::vector<Directory>	directorties;
	std::vector<File>		files;

	Directory(const std::string& inName, Directory* inParent) :
		name(inName), parent(inParent)
	{
	}

	size_t					size = 0;
	std::string				name;
	Directory* parent = nullptr;
};

void PrintDirectory(const Directory& inDir, int depth = 0)
{
	auto addTabs = [](int inDepth) {
		for (int i = 0; i < inDepth; ++i)
			std::cout << '\t';
	};

	addTabs(depth);
	std::cout << inDir.name << "\n";

	depth++;

	for(const File& file: inDir.files)
	{
		addTabs(depth);
		std::cout << file.size << " " << file.name << "\n";
	}

	for (const Directory& dir : inDir.directorties)
		PrintDirectory(dir, depth);
}

size_t RecalculateDirSizes(Directory& inDir)
{
	size_t totalSize = 0;

	for (const File& file : inDir.files)
		totalSize += file.size;

	for (Directory& dir : inDir.directorties)
		totalSize += RecalculateDirSizes(dir);

	inDir.size = totalSize;
	return totalSize;
}

size_t GetTotalSizeOfDirsToDelete(const Directory& inDir, size_t maxDirSize)
{
	size_t totalSize = 0;
	
	if (inDir.size < maxDirSize)
		totalSize += inDir.size;

	for (const Directory& dir : inDir.directorties)
		totalSize += GetTotalSizeOfDirsToDelete(dir, maxDirSize);

	return totalSize;
}
size_t GetSmallestFolderToDelete(const Directory& inDir, size_t sizeToDelete)
{
	size_t smallestFolder = inDir.size;

	if (inDir.size < sizeToDelete)
		return std::numeric_limits<size_t>::max();

	for (const Directory& dir : inDir.directorties)
		smallestFolder = std::min(smallestFolder, GetSmallestFolderToDelete(dir, sizeToDelete)); 

	return smallestFolder;
}

void GoToDir(Directory*& inCurrent, const std::string& inDirToOpen)
{
	if (inDirToOpen == "/")
	{
		while (inCurrent->parent != nullptr)
			inCurrent = inCurrent->parent;
	}
	else if (inDirToOpen == "..")
	{
		if (inCurrent->parent != nullptr)
			inCurrent = inCurrent->parent;
	}
	else
	{
		auto it = std::ranges::find_if(inCurrent->directorties, [&inDirToOpen](const Directory& inDirectory) { return inDirToOpen == inDirectory.name; });
		assert(it != inCurrent->directorties.end() && "Directory not found");
		inCurrent = &(*it);
	}
}

void ProcessFileListing(Directory*& inCurrent, const Instruction& inInstruction)
{
	for (const std::string& listing : inInstruction.instructionResults)
	{
		auto pos = listing.find("dir");
		if (pos != std::string::npos)
		{
			pos += 4;
			std::string folderName = listing.substr(pos, listing.size() - pos);
			if (std::ranges::find_if(inCurrent->directorties, [&folderName](const Directory& inDir) {
				return inDir.name == folderName;
			}) == inCurrent->directorties.end())
			{
				inCurrent->directorties.push_back(Directory(folderName, inCurrent));
			}

		}
		else
		{
			std::stringstream stringStream;
			stringStream << listing;

			size_t fileSize;
			std::string fileName;
			stringStream >> fileSize;
			stringStream >> fileName;

			if (std::ranges::find_if(inCurrent->files, [&fileName](const File& inFile) {
				return inFile.name == fileName;
			}) == inCurrent->files.end())
			{
				inCurrent->files.push_back(File(fileName, fileSize));
			}
		}
	}
}

void ProcessInstruction(const Instruction& inInstruction, Directory*& inCurrent)
{
	switch (inInstruction.type)
	{
	case Instruction::Type::nop:
		return;
	case Instruction::Type::cd:
		GoToDir(inCurrent, inInstruction.instructionContext);
		break;
	case Instruction::Type::ls:
		ProcessFileListing(inCurrent, inInstruction);
	}
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	Directory rootDir("/", nullptr);
	Directory* currentDir = &rootDir;
	Instruction instruction;
	do
	{
		instruction = GetNextInstruction(fileStream);
		ProcessInstruction(instruction, currentDir);
	} while (instruction.type != Instruction::Type::nop);
	
	PrintDirectory(rootDir);
	std::cout << "\n";
	std::cout << "Total size of Root directories: " << RecalculateDirSizes(rootDir) << "\n";
	std::cout << "Total size of directories to delete: " << GetTotalSizeOfDirsToDelete(rootDir, 100000) << "\n";
	std::cout << "Smallest size of directories to delete: " << GetSmallestFolderToDelete(rootDir, 30000000 - (70000000 - rootDir.size)) << "\n";
}
