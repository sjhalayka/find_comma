#include <iostream>
#include <string>
#include <filesystem>
#include <regex>
#include <cctype>
#include <fstream>
#include <ranges>
using namespace std;


std::string trimLeft(const std::string& str) {
	if (str.empty()) {
		return str;
	}

	size_t firstNonSpace = str.find_first_not_of(" \t\n\r\f\v");

	// If the string is all whitespace
	if (firstNonSpace == std::string::npos) {
		return "";
	}

	return str.substr(firstNonSpace);
}

std::string trimRight(const std::string& str) {
	if (str.empty()) {
		return str;
	}

	size_t lastNonSpace = str.find_last_not_of(" \t\n\r\f\v");

	// If the string is all whitespace
	if (lastNonSpace == std::string::npos) {
		return "";
	}

	return str.substr(0, lastNonSpace + 1);
}


//
//inline void trim_left_whitespace(std::string& s) {
//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
//		return !std::isspace(ch);
//		}));
//}
//
//// trim from end (in place)
//inline void trim_right_whitespace(std::string& s) {
//	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
//		return !std::isspace(ch);
//		}).base(), s.end());
//}

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
	string declaration = "";
	string filename = "";
	size_t line_number = 0;
	size_t line_pos = 0;
	long long signed int scope_depth = 0;
	//	size_t scope_block_number = 0;

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

		//if (scope_block_number < rhs.scope_block_number)
		//	return true;
		//else if (scope_block_number > rhs.scope_block_number)
		//	return false;

		return false;
	}
};


void enumerate_variables(string path, vector<variable_declaration>& declarations)
{
	declarations.clear();

	vector<string> types;

	// Note: this system does not accept variables that are
	// in the form of int***x; (note the lack of spaces)
	// So, either int*** x; or int ***x; int *** x; will work

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

	types.push_back("static* ");
	types.push_back("const* ");
	types.push_back("short* ");
	types.push_back("long* ");
	types.push_back("int* ");
	types.push_back("char* ");
	types.push_back("unsigned* ");
	types.push_back("signed* ");
	types.push_back("float* ");
	types.push_back("double* ");
	types.push_back("size_t* ");
	types.push_back("FILE* ");
	types.push_back("DIR* ");
	types.push_back("gzFile* ");
	types.push_back("struct* ");

	types.push_back("static** ");
	types.push_back("const** ");
	types.push_back("short** ");
	types.push_back("long** ");
	types.push_back("int** ");
	types.push_back("char** ");
	types.push_back("unsigned** ");
	types.push_back("signed** ");
	types.push_back("float** ");
	types.push_back("double** ");
	types.push_back("size_t** ");
	types.push_back("FILE** ");
	types.push_back("DIR** ");
	types.push_back("gzFile** ");
	types.push_back("struct** ");

	types.push_back("static*** ");
	types.push_back("const*** ");
	types.push_back("short*** ");
	types.push_back("long*** ");
	types.push_back("int*** ");
	types.push_back("char*** ");
	types.push_back("unsigned*** ");
	types.push_back("signed*** ");
	types.push_back("float*** ");
	types.push_back("double*** ");
	types.push_back("size_t*** ");
	types.push_back("FILE*** ");
	types.push_back("DIR*** ");
	types.push_back("gzFile*** ");
	types.push_back("struct*** ");

	// No quadruple pointers used in the LDAK code

	vector<string> filenames;

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		size_t str_pos = entry.path().string().find("getnums.c");

		if (str_pos != string::npos)
			filenames.push_back(entry.path().string());





		//string s = entry.path().string();

		//vector<string> tokens = std_strtok(s, "[.]\\s*");

		//for (size_t i = 0; i < tokens.size(); i++)
		//	for (size_t j = 0; j < tokens[i].size(); j++)
		//		tokens[i][j] = tolower(tokens[i][j]);

		//if (tokens.size() > 0 &&
		//	(tokens[tokens.size() - 1] == "c" ||
		//		tokens[tokens.size() - 1] == "cpp"))
		//{
		//	filenames.push_back(s);
		//}
	}




	for (size_t i = 0; i < filenames.size(); i++)
	{
		long long signed int scope_depth = 0;

		ifstream infile(filenames[i]);

		if (infile.fail())
			continue;

		string line;
		vector<string> prev_lines_vector;

		//cout << endl << endl << endl;
		//cout << filenames[i] << endl << endl;

		string type = "";
		bool inside_slashstar_comment = false;

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

			if (line[line.size() - 1] == '\\')
			{
				prev_lines_vector.push_back(line);
				continue;
			}
			else
			{
				prev_lines_vector.clear();
				prev_lines_vector.push_back(line);
			}

			//bool refresh_prev_string = true;
			bool inside_double_slash_comment = false;



			for (long signed int p = 0; p < prev_lines_vector.size(); p++)
			{ 
				//cout << "PREV_LINES_VECTOR " << prev_lines_vector[p] << endl;


				//cout << endl << endl << endl;

				//if (refresh_prev_string)
				//string prev_string = prev_lines_vector[p];

				string final_string = "";

				if (string::npos != prev_lines_vector[p].find_first_of("/*") ||
					string::npos != prev_lines_vector[p].find_first_of("*/") ||
					string::npos != prev_lines_vector[p].find_first_of("//"))
				{
					for (size_t l = 0; l < prev_lines_vector[p].size() - 1; l++)
					{
						if (false == inside_slashstar_comment && false == inside_double_slash_comment)
						{
							final_string += prev_lines_vector[p][l];
						}

						if (prev_lines_vector[p][l] == '/' && prev_lines_vector[p][l + 1] == '*')
						{
							inside_slashstar_comment = true;
							l++;

							if (final_string.size() > 0)
								final_string.pop_back();

							continue;
						}

						else if (prev_lines_vector[p][l] == '*' && prev_lines_vector[p][l + 1] == '/')
						{
							inside_slashstar_comment = false;
							l++;

							if (final_string.size() > 0)
								final_string.pop_back();

							continue;
						}

						else if (prev_lines_vector[p][l] == '/' && prev_lines_vector[p][l + 1] == '/')
						{
							inside_double_slash_comment = true;
							l++;

							if (final_string.size() > 0)
								final_string.pop_back();

							continue;
						}
					}

					if (prev_lines_vector[p].size() > 0)
						final_string += prev_lines_vector[p][prev_lines_vector[p].size() - 1];
				}
				else
				{
					final_string = prev_lines_vector[p];
				}

				if (final_string == "")
					continue;




				final_string = trimLeft(final_string);
				final_string = trimRight(final_string);


				//trim_left_whitespace(final_string);
				//trim_right_whitespace(final_string);

				if (final_string == "")
					continue;


				//cout << "FINAL_STRING \"" << final_string << "\"" << endl;



				//cout << "FINALSTRING " << final_string << endl;
				//cout << inside_slashstar_comment << " " << inside_double_slash_comment << endl;


				//if (inside_slashstar_comment || inside_double_slash_comment)
				//	final_string = prev_lines_vector[p];
				//else
				//	final_string = prev_string;








				//trim_left_whitespace(prev_lines);
				//trim_right_whitespace(prev_lines);

				bool finished_with_semi_colon = false;

				if (final_string[final_string.size() - 1] == ';')
					finished_with_semi_colon = true;
				else
				{
					//output << prev_lines << endl;
					//continue;
				}

				vector<string> statements = std_strtok(final_string, "[;]+");

				for (size_t j = 0; j < statements.size(); j++)
					statements[j] += ';';

				size_t prev_statements_location = 0;



				for (size_t s = 0; s < statements.size(); s++)
				{
					//cout << "STATEMENT " << statements[s] << endl;


					vector<string> tokens = std_strtok(statements[s], "[= \t]+");

					if (tokens.size() == 0)
						continue;

					for (size_t j = 0; j < tokens.size(); j++)
					{
						tokens[j] = trimLeft(tokens[j]);

						//trim_left_whitespace(tokens[j]);

						if (j < tokens.size() - 1)
							tokens[j] += ' ';
					}

					// Found a # or / as the first character
					if (tokens[0][0] == '#')// || tokens[0][0] == '/')
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
					bool is_static = false;

					// Is known type?
					if (types.end() != find(
						types.begin(),
						types.end(),
						tokens[0]))
					{
						//cout << "FOUND TYPE " << tokens[0] << endl;

						found_type = true;
					}
					else
						//cout << "DID NOT FIND TYPE " << tokens[0] << endl;





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
								is_static = true;

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
									if (tokens[i] == "short" ||
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
									if (tokens[i] == "unsigned" ||
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
									if (tokens[i] == "unsigned" ||
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

							//ostringstream type_oss;
							//type_oss << type << ' ';
							//size_t first_index = 1;

							//if (is_struct || is_const)
							//	first_index = 2;

							//for (size_t j = first_index; j < tokens.size(); j++)
							//	type_oss << tokens[j] << ' ';

							//type_oss << endl;

							if (false == inside_slashstar_comment)
							{
								size_t line_pos = prev_lines_vector[p].find(statements[s], prev_statements_location);

								prev_statements_location = line_pos + statements[s].size();

								long long signed int local_scope_depth = scope_depth;
								//vector<size_t> local_block_numbers = block_numbers;

								//cout << "PREV LINES P " << prev_lines_vector[p] << endl;

								long long signed int open_brace_count = count(prev_lines_vector[p].begin(), prev_lines_vector[p].begin() + line_pos, '{');
								long long signed int closing_brace_count = count(prev_lines_vector[p].begin(), prev_lines_vector[p].begin() + line_pos, '}');

								local_scope_depth += open_brace_count;
								local_scope_depth -= closing_brace_count;


								variable_declaration v;
								v.declaration = statements[s];// type_oss.str();
								v.filename = filenames[i];
								v.line_number = line_num;
									
								//if(line_pos > 0)
								v.line_pos = line_pos;// +1;

								v.scope_depth = local_scope_depth;

								declarations.push_back(v);
							}
							else
							{
								//cout << "Skipping variable declaration in comment" << endl;

								//cout << type_oss.str() << endl;
								//cout << filenames[i] << endl;
								//cout << line_num << endl;

								//cout << endl << endl;
							}
						}
					}
				}

				//cout << "LINE " << prev_lines_vector[p] << endl;

				long long signed int open_brace_count = ranges::count(prev_lines_vector[p], '{');
				long long signed int closing_brace_count = ranges::count(prev_lines_vector[p], '}');

				//long long signed int open_brace_count = count(prev_lines_vector[p].begin(), prev_lines_vector[p].end(), '{');
				//long long signed int closing_brace_count = count(prev_lines_vector[p].begin(), prev_lines_vector[p].end(), '}');

				//if (closing_brace_count != 0)
				//	cout << open_brace_count << " " << closing_brace_count << endl;

				scope_depth += open_brace_count;
				scope_depth -= closing_brace_count;

			}



		}

		infile.close();

		//cout << output.str();

		//ofstream outfile(filenames[i] + ".new");

		//outfile << output.str();// << endl;

		//outfile.close();
	}


}



void get_type_and_name(const string& input, string& variable_type0, string& variable_name0)
{
	variable_type0 = variable_name0 = "";

	vector<string> declaration_tokens0 = std_strtok(input, "[=;]+");

	if (declaration_tokens0.size() == 0)
		return;

	vector<string> declaration_tokens0_whitespace = std_strtok(declaration_tokens0[0], "[ \t]+");

	if (declaration_tokens0_whitespace.size() == 0)
		return;

	variable_name0 = declaration_tokens0_whitespace[declaration_tokens0_whitespace.size() - 1];

	variable_type0 = "";

	for (size_t j = 0; j < declaration_tokens0_whitespace.size() - 1; j++)
		variable_type0 += declaration_tokens0_whitespace[j] + " ";
}



int main(void)
{
	std::string path = "Y:/home/sjhalayka/ldak_min";

	vector<variable_declaration> declarations;

	enumerate_variables(path, declarations);

	vector<variable_declaration> pointer_only_declarations;

	for (size_t i = 0; i < declarations.size(); i++)
	{
		// TODO: move these two variables into te declaration creation

		string variable_type0 = "";
		string variable_name0 = "";

		get_type_and_name(declarations[i].declaration, variable_type0, variable_name0);

		bool found_pointer_type = false;

		// This should never happen after Microsoft style beautification of pointer types,
		// but we'll take it into account, just in case
		// e.g. variables are of the form int* x;
		if (string::npos != variable_type0.find("*"))
		{
			size_t starcount = count(variable_type0.begin(), variable_type0.end(), '*');

			variable_type0 = variable_type0.substr(0, variable_type0.size() - starcount - 1);

			for (size_t j = 0; j < starcount; j++)
				variable_name0 = '*' + variable_name0;

			found_pointer_type = true;
		}

		// This should always happen after Microsoft style beautification of pointer types
		// e.g. variables are of the form int *x;
		if (string::npos != variable_name0.find("*"))
			found_pointer_type = true;

		// We're only interested in pointers
		if (false == found_pointer_type)
			continue;

		variable_type0 = trimLeft(variable_type0);
		variable_type0 = trimRight(variable_type0);

		//trim_left_whitespace(variable_type0);
		//trim_right_whitespace(variable_type0);

		variable_name0 = trimLeft(variable_name0);
		variable_name0 = trimRight(variable_name0);

		//trim_left_whitespace(variable_name0);
		//trim_right_whitespace(variable_name0);

		pointer_only_declarations.push_back(declarations[i]);

		cout << "\"" << variable_type0 << "\"" << endl;
		cout << "\"" << variable_name0 << "\"" << endl;
		cout << "\"" << declarations[i].declaration << "\"" << endl;
		cout << "\"" << declarations[i].filename << "\"" << endl;
		cout << declarations[i].line_number << endl;
		cout << declarations[i].line_pos << endl;
		cout << declarations[i].scope_depth << endl;
		//cout << declarations[i].scope_block_number << endl;
		cout << endl << endl;
	}

	cout << declarations.size() << " " << pointer_only_declarations.size() << endl;

	//return 0;


	sort(pointer_only_declarations.begin(), pointer_only_declarations.end());

	// search for collisions
	for (size_t i = 0; i < pointer_only_declarations.size() - 1; i++)
	{
		vector<string> declaration_tokens0 = std_strtok(pointer_only_declarations[i].declaration, "[=;]+");
		vector<string> declaration_tokens1 = std_strtok(pointer_only_declarations[i + 1].declaration, "[=;]+");

		if (declaration_tokens0.size() == 0 || declaration_tokens1.size() == 0)
			continue;

		vector<string> declaration_tokens0_whitespace = std_strtok(declaration_tokens0[0], "[* \t]+");
		vector<string> declaration_tokens1_whitespace = std_strtok(declaration_tokens1[0], "[* \t]+");

		if (declaration_tokens0_whitespace.size() == 0 || declaration_tokens1_whitespace.size() == 0)
			continue;

		string variable_name0 = declaration_tokens0_whitespace[declaration_tokens0_whitespace.size() - 1];
		string variable_name1 = declaration_tokens1_whitespace[declaration_tokens1_whitespace.size() - 1];

		if (pointer_only_declarations[i].filename == pointer_only_declarations[i + 1].filename)
		{
			if (variable_name0 == variable_name1)
			{
				if (pointer_only_declarations[i].scope_depth == pointer_only_declarations[i + 1].scope_depth)
				{
					if (1)//pointer_only_declarations[i].scope_block_number == pointer_only_declarations[i + 1].scope_block_number)
					{
						cout << "possible collision" << endl;
						cout << variable_name0 << " " << variable_name1 << endl;
						cout << pointer_only_declarations[i].scope_depth << " " << pointer_only_declarations[i + 1].scope_depth << endl;
						cout << pointer_only_declarations[i].filename << endl;
						cout << endl;
					}
				}
			}
		}


		//cout << "variable name " << variable_name0 << endl;

		//cout << "declaration tokens " << endl;

		//for (size_t j = 0; j < declaration_tokens0.size(); j++)
		//{
		//	cout << declaration_tokens0[j] << ' ';
		//}

//		cout << endl;
	}

	return 0;
}