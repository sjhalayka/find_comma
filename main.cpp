#include <iostream>
#include <string>
#include <filesystem>
#include <regex>
#include <cctype>
#include <fstream>
using namespace std;


inline void trim_left_whitespace(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline void trim_right_whitespace(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

vector<string> std_strtok(const string& s, const string& regex_s)
{
	vector<string> tokens;

	regex r(regex_s);

	sregex_token_iterator iter(s.begin(), s.end(), r, -1);
	sregex_token_iterator end;

	while (iter != end)
	{
		if (*iter != "")
			tokens.push_back(*iter);

		iter++;
	}

	return tokens;
}


class variable_declaration
{
public:
	string declaration;
	string filename;
	size_t line_number;
	size_t scope_depth;
	size_t scope_block_number;

	bool operator<(const variable_declaration& rhs)
	{
		if (filename < rhs.filename)
			return true;
		else if (filename > rhs.filename)
			return false;

		if (declaration < rhs.declaration)
			return true;
		else if (declaration > rhs.declaration)
			return false;

		if (line_number < rhs.line_number)
			return true;
		else if (line_number > rhs.line_number)
			return false;

		if (scope_depth < rhs.scope_depth)
			return true;
		else if (scope_depth > rhs.scope_depth)
			return false;

		if (scope_block_number < rhs.scope_block_number)
			return true;
		else if (scope_block_number > rhs.scope_block_number)
			return false;

		return false;
	}
};


void enumerate_variables(string path, vector<variable_declaration> &declarations)
{


	vector<string> types;
	types.push_back("static ");
	types.push_back("const ");
	types.push_back("short ");
	types.push_back("long ");
	types.push_back("int ");
	types.push_back("char ");
	types.push_back("unsigned ");
	types.push_back("signed ");
	types.push_back("float ");
	types.push_back("double ");
	types.push_back("size_t ");
	types.push_back("FILE ");
	types.push_back("DIR ");
	types.push_back("gzFile ");
	types.push_back("struct ");

	vector<string> filenames;

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		string s = entry.path().string();

		vector<string> tokens = std_strtok(s, "[.]\\s*");

		for (size_t i = 0; i < tokens.size(); i++)
			for (size_t j = 0; j < tokens[i].size(); j++)
				tokens[i][j] = tolower(tokens[i][j]);

		if (tokens.size() > 0 &&
			(tokens[tokens.size() - 1] == "c" ||
				tokens[tokens.size() - 1] == "cpp"))
		{
			filenames.push_back(s);
		}
	}




	for (size_t i = 0; i < filenames.size(); i++)
	{
		size_t scope_depth = 0;
		size_t scope_block_number = 0;

		ifstream infile(filenames[i]);

		if (infile.fail())
			continue;

		string line;
		string prev_lines;

		//cout << endl << endl << endl;
		//cout << filenames[i] << endl << endl;
		string type = "";

		ostringstream output;

		size_t line_num = 0;

		while (getline(infile, line))
		{
			line_num++;

			if (line == "")
			{
				//output << endl;
				continue;
			}

			for (size_t j = 0; j < line.size(); j++)
			{
				if (line[j] == '{')
				{
					scope_block_number++;
					scope_depth++;
				}
				else if (line[j] == '}')
				{
					if (scope_depth > 0)
						scope_depth--;
				}
			}

			if (line[line.size() - 1] == '\\')
			{
				prev_lines = prev_lines + line;
				continue;
			}
			else
			{
				prev_lines = line;
			}

			//trim_left_whitespace(prev_lines);
			//trim_right_whitespace(prev_lines);

			bool finished_with_semi_colon = false;

			if (prev_lines[prev_lines.size() - 1] == ';')
				finished_with_semi_colon = true;
			else
			{
				//output << prev_lines << endl;
				continue;
			}

			vector<string> statements = std_strtok(prev_lines, "[;]\\s*");

			for (size_t j = 0; j < statements.size(); j++)
				statements[j] += ';';

			for (size_t s = 0; s < statements.size(); s++)
			{
				vector<string> tokens = std_strtok(statements[s], "[ \t]\\s*");

				if (tokens.size() == 0)
					continue;

				for (size_t j = 0; j < tokens.size(); j++)
				{
					trim_left_whitespace(tokens[j]);

					if (j < tokens.size() - 1)
						tokens[j] += ' ';
				}

				// Found a # or / as the first character
				if (tokens[0][0] == '#' || tokens[0][0] == '/')
				{
					//output << prev_lines << endl;
					break;
				}

				if (tokens[0].size() >= 3 &&
					tokens[0][0] == 'f' &&
					tokens[0][1] == 'o' &&
					tokens[0][2] == 'r')
				{
					//output << prev_lines << endl;
					break;
				}

				bool found_type = false;
				bool is_struct = false;
				bool is_const = false;
				//bool is_static = false;

				// Is known type?
				if (types.end() != find(
					types.begin(),
					types.end(),
					tokens[0]))
				{
					found_type = true;
				}

				// This is not a variable declaration statement
				if (false == found_type)
				{
					//// Not a variable declaration
					//if (finished_with_semi_colon)
					//{
					//	output << statements[s];
					//}
					//else
					//{
					//	statements[s].pop_back();
					//	output << statements[s];
					//}

					//output << endl;

					size_t malloc_found = statements[s].find("malloc");
					size_t free_found = statements[s].find("free");

					//if (malloc_found != string::npos || free_found != string::npos)
					//{
					//	output << statements[s] << endl;
					//}
					//else
					//{
					//	output << statements[s] << endl;
					//}

					continue;
				}

				//bool found_initializer = false;

				//for (size_t j = 0; j < statements[s].size(); j++)
				//{
				//	if (statements[s][j] == '{' || statements[s][j] == '}')
				//	{
				//		found_initializer = true;
				//	}
				//}

				//if (found_initializer)
				//{
				//	if (finished_with_semi_colon)
				//		output << statements[s] << endl;
				//	else
				//	{
				//		statements[s].pop_back();
				//		output << statements[s] << endl;
				//	}

				//	continue;
				//}

				//if (finished_with_comma)
				//{
				//	statements[s].pop_back();
				//	output << statements[s] << endl;
				//	cout << statements[s] << endl;
				//	continue;
				//}




				if (found_type)
				{
					if (1)//type == "" || s == 0)
					{
						type = tokens[0];

						if (tokens[0] == "static ")
						{
							//is_static = true;

							type = "static ";

							for (size_t i = 1; i < tokens.size(); i++)
							{
								if (//tokens[i] == "static " ||
									tokens[i] == "size_t " ||
									tokens[i] == "FILE " ||
									tokens[i] == "DIR " ||
									tokens[i] == "gzFile " ||
									tokens[i] == "double " ||
									tokens[i] == "float " ||
									tokens[i] == "unsigned " ||
									tokens[i] == "signed " ||
									tokens[i] == "short " ||
									tokens[i] == "long " ||
									tokens[i] == "int " ||
									tokens[i] == "char ")
								{
									/*if (tokens[i] != "static ")*/
									type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "const")
						{
							type = "const ";

							for (size_t i = 0; i < tokens.size(); i++)
							{
								if (tokens[i] == "size_t" ||
									tokens[i] == "FILE" ||
									tokens[i] == "DIR" ||
									tokens[i] == "gzFile" ||
									tokens[i] == "double" ||
									tokens[i] == "float" ||
									tokens[i] == "unsigned" ||
									tokens[i] == "signed" ||
									tokens[i] == "short" ||
									tokens[i] == "long" ||
									tokens[i] == "int" ||
									tokens[i] == "char")
								{
									if (tokens[i] != "const")
										type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "unsigned")
						{
							type = "unsigned ";

							for (size_t i = 0; i < tokens.size(); i++)
							{
								if (tokens[i] == "short" ||
									tokens[i] == "long" ||
									tokens[i] == "int" ||
									tokens[i] == "char")
								{
									if (tokens[i] != "unsigned")
										type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "signed")
						{
							type = "signed ";

							for (size_t i = 1; i < tokens.size(); i++)
							{
								if (
									tokens[i] == "short" ||
									tokens[i] == "long" ||
									tokens[i] == "int" ||
									tokens[i] == "char")
								{
									if (tokens[i] != "signed")
										type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "short")
						{
							type = "short ";

							for (size_t i = 0; i < tokens.size(); i++)
							{
								if (
									tokens[i] == "unsigned" ||
									tokens[i] == "signed" ||
									tokens[i] == "int")
								{
									if (tokens[i] != "short")
										type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "long ")
						{

							type = "long ";

							for (size_t i = 0; i < tokens.size(); i++)
							{
								if (
									tokens[i] == "unsigned" ||
									tokens[i] == "signed" ||
									tokens[i] == "int")
								{
									if (tokens[i] != "long ")
										type += tokens[i] + " ";

									tokens.erase(tokens.begin() + i);
									i = 0;
								}
							}
						}
						else if (tokens.size() > 1 && tokens[0] == "struct")
						{
							is_struct = true;
							type = "struct " + tokens[1];
						}

						ostringstream type_oss;
						type_oss << type << ' ';
						size_t first_index = 1;

						if (is_struct || is_const)
							first_index = 2;

						for (size_t j = first_index; j < tokens.size(); j++)
							type_oss << tokens[j] << ' ';

						//type_oss << endl;

						variable_declaration v;
						v.declaration = type_oss.str();
						v.filename = filenames[i];
						v.line_number = line_num;
						v.scope_depth = scope_depth;
						v.scope_block_number = scope_block_number;

						declarations.push_back(v);
					}
				}
			}
		}

		infile.close();

		//cout << output.str();

		//ofstream outfile(filenames[i] + ".new");

		//outfile << output.str();// << endl;

		//outfile.close();
	}


}



int main(void)
{
	std::string path = "Y:/home/sjhalayka/ldak_min";

	vector<variable_declaration> declarations;

	enumerate_variables(path, declarations);

	//for (size_t i = 0; i < declarations.size(); i++)
	//{
	//	cout << declarations[i].declaration << endl;
	//	cout << declarations[i].filename << endl;
	//	cout << declarations[i].line_number << endl;
	//	cout << declarations[i].scope_depth << endl;

	//	cout << endl;
	//}

	//return 0;

	sort(declarations.begin(), declarations.end());

	// Search for collisions
	for (size_t i = 0; i < declarations.size() - 1; i++)
	{
		vector<string> declaration_tokens0 = std_strtok(declarations[i].declaration, "[=;]\\s*");
		vector<string> declaration_tokens1 = std_strtok(declarations[i + 1].declaration, "[=;]\\s*");

		if (declaration_tokens0.size() == 0 || declaration_tokens1.size() == 0)
			continue;

		vector<string> declaration_tokens0_whitespace = std_strtok(declaration_tokens0[0], "[ \t]\\s*");
		vector<string> declaration_tokens1_whitespace = std_strtok(declaration_tokens1[0], "[ \t]\\s*");

		if (declaration_tokens0_whitespace.size() == 0 || declaration_tokens1_whitespace.size() == 0)
			continue;

		string variable_name0 = declaration_tokens0_whitespace[declaration_tokens0_whitespace.size() - 1];
		string variable_name1 = declaration_tokens1_whitespace[declaration_tokens1_whitespace.size() - 1];

		if (declarations[i].filename == declarations[i + 1].filename)
		{
			if (variable_name0 == variable_name1)
			{
				if (declarations[i].scope_depth == declarations[i + 1].scope_depth)
				{
					cout << "Possible collision" << endl;
					cout << variable_name0 << " " << variable_name1 << endl;
					cout << declarations[i].scope_block_number << " " << declarations[i + 1].scope_block_number << endl;
					cout << declarations[i].filename << endl;
					cout << endl;

					//if (declarations[i].scope_block_number == declarations[i + 1].scope_block_number)
					//{

					//}
				}
			}
		}


		//cout << "VARIABLE NAME " << variable_name0 << endl;

		//cout << "DECLARATION TOKENS " << endl;

		//for (size_t j = 0; j < declaration_tokens0.size(); j++)
		//{
		//	cout << declaration_tokens0[j] << ' ';
		//}

//		cout << endl;
	}

	return 0;
}