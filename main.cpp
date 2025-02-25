#include <iostream>
#include <string>
#include <filesystem>
#include <regex>
#include <cctype>
#include <fstream>
#include <ranges>
#include <map>
#include <set>
using namespace std;


const size_t num_chars_in_random_strings = 16;

string generateUniqueRandomString(const size_t length)
{
	static vector<string> past_used_random_strings;
	static const char* const charmap = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const size_t charmapLength = strlen(charmap);

	string result = "";

	while (1)
	{
		for (size_t l = 0; l < length; l++)
			result += charmap[rand() % charmapLength];

		const vector<string>::const_iterator ci = find(past_used_random_strings.begin(), past_used_random_strings.end(), result);

		if (ci != past_used_random_strings.end())
		{
			// collision, try again
			result = "";
			continue;
		}
		else
		{
			// no collision, proceed
			past_used_random_strings.push_back(result);
			break;
		}
	}

	return result;
}



// returns count of non-overlapping occurrences of 'sub' in 'str'
int countSubstring(const std::string& str, const std::string& sub)
{
	if (sub.length() == 0) return 0;
	int count = 0;
	for (size_t offset = str.find(sub); offset != std::string::npos;
		offset = str.find(sub, offset + sub.length()))
	{
		++count;
	}
	return count;
}


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
	string var_name = "";
	string var_type = "";
	string filename = "";
	size_t line_number = 0;
	size_t line_pos = 0;
	long long signed int scope_depth = 0;
	string scope_id = "";

	bool operator<(const variable_declaration& rhs)
	{
		//if (filename < rhs.filename)
		//	return true;
		//else if (filename > rhs.filename)
		//	return false;

		if (var_name < rhs.var_name)
			return true;
		else if (var_name > rhs.var_name)
			return false;

		//if (declaration < rhs.declaration)
		//	return true;
		//else if (declaration > rhs.declaration)
		//	return false;

		//if (line_number < rhs.line_number)
		//	return true;
		//else if (line_number > rhs.line_number)
		//	return false;

		if (scope_depth < rhs.scope_depth)
			return true;
		else if (scope_depth > rhs.scope_depth)
			return false;

		//if (scope_id < rhs.scope_id)
		//	return true;
		//else if (scope_id > rhs.scope_id)
		//	return false;

		return false;
	}
};



class non_variable_declaration
{
public:
	string declaration = "";
	string var_name = "";
	string filename = "";
	size_t line_number = 0;
	size_t line_pos = 0;
	string scope_id = "";
	long long signed int scope_depth = 0;

	bool operator<(const non_variable_declaration& rhs)
	{
		if (filename < rhs.filename)
			return true;
		else if (filename > rhs.filename)
			return false;

		if (var_name < rhs.var_name)
			return true;
		else if (var_name > rhs.var_name)
			return false;

		if (line_number < rhs.line_number)
			return true;
		else if (line_number > rhs.line_number)
			return false;

		if (line_pos < rhs.line_pos)
			return true;
		else if (line_pos > rhs.line_pos)
			return false;

		return false;
	}
};




vector<string> get_filenames(const string path)
{
	vector<string> filenames;

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		//size_t str_pos = entry.path().string().find("sort.c");

		//if (str_pos != string::npos)
		//	filenames.push_back(entry.path().string());







		size_t str_pos = entry.path().string().find("declaration.c");

		if (str_pos != string::npos)
			filenames.push_back(entry.path().string());

		string s = entry.path().string();

		vector<string> tokens = std_strtok(s, "[.]\\s*");

		for (size_t i = 0; i < tokens.size(); i++)
			for (size_t j = 0; j < tokens[i].size(); j++)
				tokens[i][j] = tolower(tokens[i][j]);

		if (tokens.size() > 0 &&
			(tokens[tokens.size() - 1] == "c" ||
				tokens[tokens.size() - 1] == "cpp"))
		{
			size_t str_pos = s.find("declaration.c");

			if (str_pos == string::npos)
			{
				filenames.push_back(s);
			}
			else
			{
				//cout << "skipping declaration.c because we already included it above" << endl;
			}
		}



	}

	return filenames;
}


vector<string> get_variable_types(void)
{
	vector<string> types;

	// No quadruple pointers used in the LDAK code

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

	return types;
}



void enumerate_non_variables(const string path, const vector<variable_declaration>& declarations, vector<non_variable_declaration>& non_declarations)
{
	//srand(456);

	non_declarations.clear();

	vector<string> types = get_variable_types();
	vector<string> filenames = get_filenames(path);

	for (size_t i = 0; i < filenames.size(); i++)
	{
		vector<string> var_names;

		for (size_t j = 0; j < declarations.size(); j++)
		{
			//if (declarations[j].filename == filenames[i])
			var_names.push_back(declarations[j].var_name);
		}



		long long signed int scope_depth = 0;
		vector<string> scope_ids;

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



				vector<string> statements = std_strtok(final_string, "[{};]+");

				for (size_t j = 0; j < statements.size(); j++)
				{
					if (j == statements.size() - 1 && false == finished_with_semi_colon)
					{

					}
					else
					{
						statements[j] += ';';
					}
				}

				//if (finished_with_semi_colon == false)
				//	statements[statements.size() - 1].pop_back();



				long long signed int open_brace_count = ranges::count(prev_lines_vector[p], '{');
				long long signed int closing_brace_count = ranges::count(prev_lines_vector[p], '}');

				scope_depth += open_brace_count;
				scope_depth -= closing_brace_count;

				for (long long signed int j = 0; j < open_brace_count; j++)
					scope_ids.push_back(generateUniqueRandomString(num_chars_in_random_strings));

				for (long long signed int j = 0; j < closing_brace_count; j++)
					scope_ids.pop_back();



				size_t prev_statements_location = 0;






				for (size_t s = 0; s < statements.size(); s++)
				{
					string temp_string = statements[s];

					// Add padding spacing around things like int***x;
					temp_string = regex_replace(temp_string, regex("(\\*+)"), " $1 ");
					temp_string = regex_replace(temp_string, regex("\\s+"), " ");

					vector<string> tokens = std_strtok(temp_string, "[= \t]+");

					if (tokens.size() == 0)
						continue;

					for (size_t j = 0; j < tokens.size(); j++)
					{
						tokens[j] = trimLeft(tokens[j]);

						//trim_left_whitespace(tokens[j]);

						if (j < tokens.size() - 1)
							tokens[j] += ' ';
					}

					bool check_for_type = true;

					if (tokens[0][0] == '#')
					{
						check_for_type = false;
					}

					if (tokens[0].size() >= 3 &&
						tokens[0][0] == 'f' &&
						tokens[0][1] == 'o' &&
						tokens[0][2] == 'r')
					{
						check_for_type = false;
					}

					bool found_type = false;

					// Is known type?
					if (check_for_type && types.end() != find(
						types.begin(),
						types.end(),
						tokens[0]))
					{
						//cout << "FOUND TYPE " << tokens[0] << endl;

						found_type = true;
					}


					// This is not a variable declaration statement
					if (false == found_type)
					{
						// Tokenize by stripping whitespace, all operators, and parentheses, etc.
						vector<string> tokens = std_strtok(statements[s], "[!~^ \t;:.,\\-\\+\\*/<=>&\\(\\)\\[\\]\\{\\}]\\s*");

						//cout << statements[s] << endl;

						for (size_t t = 0; t < tokens.size(); t++)
						{
							string token = tokens[t];

							const vector<string>::const_iterator ci = find(var_names.begin(), var_names.end(), token);

							if (var_names.end() != ci)
							{
								size_t statement_line_pos = prev_lines_vector[p].find(statements[s]);
								size_t token_statement_line_pos = statements[s].find(token);

								size_t line_pos = statement_line_pos + token_statement_line_pos;

								//cout << "VAR_NAME " << *ci << endl;
								//cout << "LINE_NUM " << line_num << endl;

								//if (line_pos == string::npos)
								//	cout << "LINE_POS NPOS" << endl;
								//else
								//	cout << "LINE_POS " << line_pos << endl;

								non_variable_declaration nvd;
								nvd.var_name = *ci;
								nvd.declaration = statements[s];
								nvd.filename = filenames[i];
								nvd.line_number = line_num;
								nvd.line_pos = line_pos;
								nvd.scope_depth = scope_depth;

								//nvd.scope_id = declarations[x].filename
								for (size_t x = 0; x < declarations.size(); x++)
								{
									if (nvd.filename == declarations[x].filename &&
										nvd.var_name == declarations[x].var_name &&
										nvd.line_number < declarations[x].line_number)
									{
										break;
									}
									else if (nvd.filename == declarations[x].filename &&
										nvd.var_name == declarations[x].var_name &&
										nvd.scope_depth >= declarations[x].scope_depth)
									{
										nvd.scope_id = declarations[x].scope_id;
									}
								}
								
								non_declarations.push_back(nvd);
							}

							///cout << token << endl;
						}
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





void enumerate_variables(const string path, vector<variable_declaration>& declarations)
{
//	srand(123);

	declarations.clear();

	vector<string> types = get_variable_types();
	vector<string> filenames = get_filenames(path);

	for (size_t i = 0; i < filenames.size(); i++)
	{
		long long signed int scope_depth = 0;

		vector<string> scope_ids;

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

				vector<string> statements = std_strtok(final_string, "[{};]+");

				for (size_t j = 0; j < statements.size(); j++)
					statements[j] += ';';

				size_t prev_statements_location = 0;


				for (size_t s = 0; s < statements.size(); s++)
				{



					//string statement_backup = statements[s];

					//for (size_t k = 0; k < statement_backup.size(); k++)
					//{
					//	if (statement_backup[k] == '{' || statement_backup[k] == '}')
					//		statement_backup[k] = ' ';
					//}

					//if(statement_backup.size() > 0)
					//statements[s] = statement_backup;


					//cout << "STATEMENT " << statements[s] << endl;
					//cout << "STATEMENT BACKUP " << statement_backup << endl;
					//string temp_string = statements[s];

					//string temp_string2 = "";

					//for (size_t j = 0; j < temp_string.size() - 1; j++)
					//{
					//	temp_string2 += temp_string[j];

					//	if (temp_string[j] != '*' && temp_string[j + 1] == '*')
					//	{
					//		temp_string2 += ' ';
					//	}


					//}

					//temp_string2 += temp_string[temp_string.size() - 1];


					//
					//string temp_string2 = regex_replace(temp_string, regex("\\s+"), " ");


					string temp_string = statements[s];

					temp_string = regex_replace(temp_string, regex("(\\*+)"), " $1 ");
					temp_string = regex_replace(temp_string, regex("\\s+"), " ");


					//cout << "STATEMENT BEFORE " << statements[s] << endl;


					//// only do this if there is no space after the stars
					//size_t first_star_pos = temp_string.find_first_of('*');

					//if (first_star_pos != string::npos)
					//{
					//	if (first_star_pos >= 1)
					//	{
					//		if (temp_string[first_star_pos - 1] != ' ')// && temp_string[first_star_pos - 1] != '\t')
					//		{
					//			temp_string.insert(temp_string.begin() + first_star_pos, ' ');
					//		}
					//	}
					//}
					//


					//cout << "STATEMENT AFTER " << temp_string << endl;


					//bool found_non_star_after_star


			/*		string temp_string2 = regex_replace(statements[s], regex("(\\*+)"), "$1 ");
					temp_string2 = regex_replace(temp_string2, regex("\\s+"), " ");

					temp_string2 = trimLeft(temp_string2);
					temp_string2 = trimRight(temp_string2);*/



					//cout << "STATEMENTS " << statements[s] << endl;
					//cout << "tempstring2 " << temp_string2 << endl;





					vector<string> tokens = std_strtok(temp_string, "[= \t]+");

					if (tokens.size() == 0)
						continue;




					for (size_t j = 0; j < tokens.size(); j++)
					{
						tokens[j] = trimLeft(tokens[j]);

						//trim_left_whitespace(tokens[j]);

						if (j < tokens.size() - 1)
							tokens[j] += ' ';
					}



					//// Perform the replaceme

					//string temp_string = regex_replace(tokens[0], regex("(\\*+)"), " $1 ");


					//string temp_string2 = regex_replace(temp_string, regex("\\s+"), " ");

					//vector<string> temp_tokens = std_strtok(temp_string2, "[= \t]+");

					//cout << "TOKENS" << endl;

					//for (size_t j = 0; j < temp_tokens.size(); j++)
					//	cout << temp_tokens[j] << endl;

					//cout << endl;


					//string temp_string = tokens[0];
					//size_t star_count = 0;

					//string temp_string2 = "";

					//for (size_t j = 0; j < temp_string.size() - 1; j++)
					//{
					//	if (temp_string[j] == '*' && temp_string[j + 1] != '*')
					//	{
					//		temp_string2 += temp_string[j];
					//		temp_string2 += ' ';
					//		
					//	}
					//	else
					//	{
					//		temp_string2 += temp_string[j];
					//	}
					//}


					//if(temp_string[temp_string.size() - 1] != ';')
					//temp_string2 += temp_string[temp_string.size() - 1];

					//	cout << "TOKENS[0]   " << tokens[0] << endl;
					//	cout << "TEMP STRING2 " << temp_string2 << endl;


					//tokens[0] = temp_string2;






					//string temp_string = "";
					//size_t star_count = 0;

					//for (size_t j = 0; j < tokens[0].size() - 1; j++)
					//{
					//	temp_string += tokens[0][j];

					//	if (tokens[0][j] != '*' && tokens[0][j + 1] == '*')
					//		temp_string += ' ';
					//	else if (tokens[0][j] == '*' && tokens[0][j + 1] != '*')
					//		temp_string += ' ';

					//	if (tokens[0][j] == '*')
					//		star_count++;
					//}

					//if (tokens[0][tokens[0].size() - 1] == '*')
					//	star_count++;

					//temp_string += tokens[0][tokens[0].size() - 1];

					//temp_string = trimLeft(temp_string);
					//temp_string = trimRight(temp_string);

					////cout << "TOKENS[0]   " << tokens[0] << endl;
					////cout << "TEMP STRING " << temp_string << endl;
					//
					//if(star_count)
					//tokens[0] = temp_string;


					// to do: add space before and after stars where not adjacent


					if (tokens[0][0] == '#')
					{
						//output << prev_lines << endl;
						continue;
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
					//else
					//	cout << "DID NOT FIND TYPE " << tokens[0] << endl;





					// This is not a variable declaration statement
					if (false == found_type)
					{
						//cout << "DIDNT FIND TYPE " << tokens[0] << endl;

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

							if (tokens.size() > 1 && tokens[0] == "static")
							{
								is_static = true;


								type = "static ";

								for (size_t i = 0; i < tokens.size(); i++)
								{
									if (tokens[i] == "size_t " ||
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
										if (tokens[i] != "static")
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
								// Avoid trouble with things like "double func_name()"
								// and things like void func(double x) and
								// and things like void func(double x, double y)
								if (string::npos != statements[s].find('(') ||
									string::npos != statements[s].find(')') ||
									string::npos != statements[s].find(','))
									continue;

								string temp_statement = statements[s];

								if (temp_statement.size() > 0 && temp_statement[temp_statement.size() - 1] == ';')
									temp_statement.pop_back();

								statements[s] = temp_statement;

								statements[s] = trimLeft(statements[s]);
								statements[s] = trimRight(statements[s]);

								size_t line_pos = prev_lines_vector[p].find(statements[s], prev_statements_location);

								if (line_pos == string::npos)
								{
									cout << "COULD NOT FIND STATEMENT " << endl;
									cout << prev_lines_vector[p] << endl;
									cout << statements[s] << endl;
									cout << endl;
									//exit(123);
								}

								prev_statements_location = line_pos + statements[s].size();

								long long signed int local_scope_depth = scope_depth;
								//cout << "PREV LINES P " << prev_lines_vector[p] << endl;
								vector<string> local_scope_ids = scope_ids;

								long long signed int open_brace_count = ranges::count(prev_lines_vector[p].begin(), prev_lines_vector[p].begin() + line_pos, '{');
								long long signed int closing_brace_count = ranges::count(prev_lines_vector[p].begin(), prev_lines_vector[p].begin() + line_pos, '}');

								local_scope_depth += open_brace_count;
								local_scope_depth -= closing_brace_count;

								for (long long signed int j = 0; j < open_brace_count; j++)
									local_scope_ids.push_back(generateUniqueRandomString(num_chars_in_random_strings));

								for (long long signed int j = 0; j < closing_brace_count; j++)
									local_scope_ids.pop_back();

								variable_declaration v;
								v.declaration = statements[s] + ';';// type_oss.str();
								v.filename = filenames[i];
								v.line_number = line_num;
								v.line_pos = line_pos;
								v.scope_depth = local_scope_depth;

								if (local_scope_ids.size() > 0)
									v.scope_id = local_scope_ids[local_scope_ids.size() - 1];
								else
								{
									//v.scope_id = "TEST";
									local_scope_ids.push_back(generateUniqueRandomString(num_chars_in_random_strings));
									v.scope_id = local_scope_ids[local_scope_ids.size() - 1];
								}

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

				scope_depth += open_brace_count;
				scope_depth -= closing_brace_count;

				for (long long signed int j = 0; j < open_brace_count; j++)
					scope_ids.push_back(generateUniqueRandomString(num_chars_in_random_strings));

				for (long long signed int j = 0; j < closing_brace_count; j++)
					scope_ids.pop_back();
			}



		}

		infile.close();

		//cout << output.str();

		//ofstream outfile(filenames[i] + ".new");

		//outfile << output.str();// << endl;

		//outfile.close();
	}


}



void get_type_and_name(string input, string& variable_type0, string& variable_name0)
{
	input = trimLeft(input);
	input = trimRight(input);

	input = regex_replace(input, regex("(\\*+)"), " $1 ");
	input = regex_replace(input, regex("\\s+"), " ");

	variable_type0 = variable_name0 = "";

	vector<string> declaration_tokens0 = std_strtok(input, "[=;]+");

	if (declaration_tokens0.size() == 0)
		return;

	vector<string> declaration_tokens0_whitespace = std_strtok(declaration_tokens0[0], "[ \t]+");

	if (declaration_tokens0_whitespace.size() == 0)
		return;

	variable_name0 = declaration_tokens0_whitespace[declaration_tokens0_whitespace.size() - 1];

	string temp_name = "";

	size_t num_stars_found = 0;

	for (size_t i = 0; i < variable_name0.size(); i++)
	{
		if (variable_name0[i] != '*')
			temp_name += variable_name0[i];
		else
			num_stars_found++;
	}

	variable_name0 = temp_name;

	variable_type0 = "";

	for (size_t j = 0; j < declaration_tokens0_whitespace.size() - 1; j++)
	{
		variable_type0 += declaration_tokens0_whitespace[j] + ' ';
	}

	for (size_t j = 0; j < num_stars_found; j++)
	{
		variable_type0 += '*';
	}

	variable_type0 += ' ';

	//cout << "VAR_NAME " << variable_name0 << endl;

	//cout << "VAR_TYPE " << variable_type0 << endl;
}




int main(void)
{
	srand(0);
	
	//std::string path = "Y:/home/sjhalayka/ldak_min";
	std::string path = "Y:/home/sjhalayka/input_code";

	vector<variable_declaration> declarations;

	enumerate_variables(path, declarations);

	if (declarations.size() == 0)
	{
		cout << "No declarations" << endl;
		return -1;
	}


	vector<variable_declaration> pointer_only_declarations;

	for (size_t i = 0; i < declarations.size(); i++)
	{
		string variable_type0 = "";
		string variable_name0 = "";

		get_type_and_name(declarations[i].declaration, variable_type0, variable_name0);

		declarations[i].var_name = variable_name0;
		declarations[i].var_type = variable_type0;

		bool found_pointer_type = false;

		// This should never happen after Microsoft style beautification of pointer types,
		if (string::npos != variable_type0.find("*"))
		{
			//size_t starcount = count(variable_type0.begin(), variable_type0.end(), '*');

			//variable_type0 = variable_type0.substr(0, variable_type0.size() - starcount - 1);

			//for (size_t j = 0; j < starcount; j++)
			//	variable_name0 = '*' + variable_name0;

			//cout << "FOUND STARS ON TYPE" << endl;

			found_pointer_type = true;
		}

		// This should always happen after Microsoft style beautification of pointer types
		// e.g. variables are of the form int *x;
		else if (string::npos != variable_name0.find("*"))
		{
			size_t starcount = count(variable_name0.begin(), variable_name0.end(), '*');

			variable_name0 = variable_name0.substr(1, variable_name0.size() - starcount);

			for (size_t j = 0; j < starcount; j++)
				variable_type0 = variable_type0 + '*';

			found_pointer_type = true;
		}

		// We're only interested in pointers
		if (false == found_pointer_type)
			continue;

		variable_type0 = trimLeft(variable_type0);
		variable_type0 = trimRight(variable_type0);

		variable_name0 = trimLeft(variable_name0);
		variable_name0 = trimRight(variable_name0);

		//variable_declaration temp_;
		//temp_.var_name = variable_name0;
		//temp_.var_type = variable_type0;
		//temp_.declaration = declarations[i].declaration;
		//temp_.filename = declarations[i].filename;
		//temp_.line_number = declarations[i].line_number;
		//temp_.line_pos = declarations[i].line_pos;
		//temp_.scope_depth = declarations[i].scope_depth;
		//temp_.scope_id = declarations[i].scope_id;

		pointer_only_declarations.push_back(declarations[i]);

		//cout << "\"" << variable_type0 << "\"" << endl;
		//cout << "\"" << variable_name0 << "\"" << endl;
		//cout << "\"" << declarations[i].declaration << "\"" << endl;
		//cout << "\"" << declarations[i].filename << "\"" << endl;
		//cout << declarations[i].line_number << endl;
		//cout << declarations[i].line_pos << endl;
		//cout << declarations[i].scope_depth << endl;
		//cout << endl << endl;
	}

	cout << "Declaration count: " << declarations.size() << endl;
	cout << "Pointer declaration count: " << pointer_only_declarations.size() << endl;
	cout << endl;

	if (pointer_only_declarations.size() == 0)
	{
		cout << "No pointer declarations" << endl;
		return -1;
	}

	sort(pointer_only_declarations.begin(), pointer_only_declarations.end());

	// search for collisions
	for (size_t i = 0; i < pointer_only_declarations.size() - 1; i++)
	{
		if (pointer_only_declarations[i].filename == pointer_only_declarations[i + 1].filename)
		{
			string variable_name0 = pointer_only_declarations[i].var_name;
			string variable_name1 = pointer_only_declarations[i + 1].var_name;

			if (variable_name0 == variable_name1)
			{
				if (pointer_only_declarations[i].scope_depth == pointer_only_declarations[i + 1].scope_depth)
				{
					if (pointer_only_declarations[i].scope_id == pointer_only_declarations[i + 1].scope_id)
					{
						cout << "Possible collision:" << endl;
						cout << variable_name0 << " " << variable_name1 << endl;
						cout << pointer_only_declarations[i].scope_depth << " " << pointer_only_declarations[i + 1].scope_depth << endl;
						cout << pointer_only_declarations[i].filename << endl;
						cout << pointer_only_declarations[i].line_number << endl;
						cout << endl;
					}
				}
			}
		}
	}

	 cout << endl;


	//map<string, size_t> type_map;

	//for (size_t i = 0; i < pointer_only_declarations.size(); i++)
	//	type_map[pointer_only_declarations[i].var_type]++;

	//for (map<string, size_t>::const_iterator i = type_map.begin(); i != type_map.end(); i++)
	//	cout << i->first << " " << i->second << endl;



	//for (size_t i = 0; i < pointer_only_declarations.size(); i++)
	//{
	//	cout << pointer_only_declarations[i].declaration << endl;
	//}

	//cout << endl;



	vector<non_variable_declaration> non_declarations;

	enumerate_non_variables(path, pointer_only_declarations, non_declarations);

	cout << "Non declarations" << endl;

	for (size_t i = 0; i < non_declarations.size(); i++)
		cout << non_declarations[i].declaration << endl;

	cout << endl;


	//sort(non_declarations.begin(), non_declarations.end());

	map<string, size_t> variable_use_counts;
	map<string, size_t> malloc_counts;
	map<string, size_t> free_counts;

	for (size_t i = 0; i < non_declarations.size(); i++)
	{
		const size_t var_name_location = non_declarations[i].declaration.find(non_declarations[i].var_name);
		const size_t malloc_location = non_declarations[i].declaration.find("malloc");
		const size_t free_location = non_declarations[i].declaration.find("free");

		const string s = non_declarations[i].filename + "::" + to_string(non_declarations[i].scope_depth) + "::" + non_declarations[i].scope_id + "::" + non_declarations[i].var_name;

		if (var_name_location != string::npos)
			variable_use_counts[s]++;

		if(malloc_location != string::npos)
			malloc_counts[s]++;

		if (free_location != string::npos)
			free_counts[s]++;
	}

//	cout << "NONDECLARATIONS" << endl;

	cout << "Variables used: " << variable_use_counts.size() << endl;
	cout << endl;
	 



	cout << "References" << endl << endl;

	for (map<string, size_t>::const_iterator ci = variable_use_counts.begin(); ci != variable_use_counts.end(); ci++)
	{
		if (ci->second != 0)
		{
			size_t m = malloc_counts[ci->first];
			size_t f = free_counts[ci->first];

			if (m != 0 || f != 0)
				//if (/*(m == 0 && f != 0) ||*/ (m != 0 && f == 0))
				//if (m != f && !(m == 0 && f == 1))
			{
				cout << ci->first << endl;// " " << ci->second << endl;
				cout << "malloc() calls " << m << endl;
				cout << "free() calls " << f << endl;
				cout << "total references " << variable_use_counts[ci->first] << endl;
				cout << "total references minus malloc and free " << variable_use_counts[ci->first] - m - f << endl;
				
				cout << endl << endl;
			}
			else
			{
				cout << "No malloc() or free() found for " << ci->first << endl;
				cout << "total references " << variable_use_counts[ci->first] << endl;
				cout << endl << endl;
			}
		}
		else
		{
			cout << "Skipping unused variable " << ci->first << endl;
			cout << endl << endl;
		}

		//		cout << endl;
	}

	//cout << endl;

	//cout << "malloc() calls" << endl;

	//for (map<string, size_t>::const_iterator ci = malloc_counts.begin(); ci != malloc_counts.end(); ci++)
	//{
	//	if (ci->second != 0)
	//		cout << ci->first << " " << ci->second << endl;
	//}

	//cout << endl;

	//cout << "free() calls" << endl;

	//for (map<string, size_t>::const_iterator ci = free_counts.begin(); ci != free_counts.end(); ci++)
	//{
	//	if (ci->second != 0)
	//		cout << ci->first << " " << ci->second << endl;
	//}

	//cout << endl;

	return 0;
}