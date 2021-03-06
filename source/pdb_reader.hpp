/**
 * Copyright (C) 2016 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/blink#license
 */

#pragma once

#include "msf_reader.hpp"
#include <unordered_map>

namespace blink
{
	/// <summary>
	/// A global unique identifier (GUID).
	/// </summary>
	struct guid
	{
		uint32_t data1;
		uint32_t data2;
		uint32_t data3;
		uint32_t data4;

		bool operator==(const guid &other) { return data1 == other.data1 && data2 == other.data2 && data3 == other.data3 && data4 == other.data4; }
		bool operator!=(const guid &other) { return !operator==(other); }
	};

	/// <summary>
	/// Class which reads a program debug database.
	/// </summary>
	class pdb_reader : public msf_reader
	{
	public:
		/// <summary>
		/// Opens a program debug database file.
		/// </summary>
		/// <param name="path">The file system path the PDB file is located at.</param>
		explicit pdb_reader(const std::string &path);

		/// <summary>
		/// Returns the PDB file version.
		/// </summary>
		unsigned int version() const { return _version; }
		/// <summary>
		/// Returns the date time stamp at which the PDB file was created.
		/// </summary>
		unsigned int timestamp() const { return _timestamp; }
		/// <summary>
		/// Returns the GUID of this PDB file for matching it to its executable image file.
		/// </summary>
		guid guid() const { return _guid; }

		using msf_reader::stream;
		/// <summary>
		/// Gets a named content stream.
		/// </summary>
		/// <param name="name">The name of the stream.</param>
		std::vector<char> stream(const std::string &name)
		{
			const auto it = _named_streams.find(name);

			if (it == _named_streams.end())
				return {};

			return msf_reader::stream(it->second);
		}

		/// <summary>
		/// Walks through all symbols in this PDB file and returns them.
		/// </summary>
		std::unordered_map<std::string, void *> symbols(uint8_t *image_base);
		/// <summary>
		/// Walks through the build info in this PDB file and returns all source file paths.
		/// </summary>
		std::vector<std::string> sourcefiles();
		/// <summary>
		/// Returns the hash table of names found in the PDB file.
		/// </summary>
		std::unordered_map<unsigned int, std::string> names();

	private:
		unsigned int _version = 0, _timestamp = 0;
		struct guid _guid = {};
		std::unordered_map<std::string, unsigned int> _named_streams;
	};
}
