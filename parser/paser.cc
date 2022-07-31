
#include "parser.h"
#include <cstring>
#include <iostream>

/**
 * takes a string in the Format "c i i i ..."
 * (1 char followed by potentially many integers) and returns a vector of all i
 * @param str the string to tokenize
 * @param tokens the result vector of wire id
 */
void
tokenize_verilog (const std::string &str, std::vector<uint32_t> &tokens,
                  const std::string &delimiters)
{

  tokens.clear ();

  // Skip delimiters at beginning. Skip first two characters (1 Char + 1 Space)
  std::string::size_type lastPos = str.find_first_not_of (delimiters, 2);

  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of (delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back (atoi (str.substr (lastPos, pos - lastPos).c_str ()));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of (delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of (delimiters, lastPos);
    }
}

/**
 * takes a string in the Format "i|i|i|..."
 * (integers separated by '|') and returns a vector of all integers
 * @param str the string to tokenize
 * @param tokens the result vector of wire id
 */
void
tokenize (const std::string &str, std::vector<uint32_t> &tokens,
          const std::string &delimiters)
{

  tokens.clear ();

  // Skip delimiters at beginning
  std::string::size_type lastPos = str.find_first_not_of (delimiters, 0);

  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of (delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back (atoi (str.substr (lastPos, pos - lastPos).c_str ()));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of (delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of (delimiters, lastPos);
    }
}

// really confusing
int32_t
parse_options (int32_t *argcp, char ***argvp, parsing_ctx *options,
               uint32_t nops)
{
  int result = 0; // counter for sucessfully parsed param
  bool skip;
  uint32_t i;

  // no params
  if (*argcp < 2)
    return 0;

  while ((*argcp) > 1)
    {
      // only support single letter flag ??
      if ((*argvp)[1][0] != '-' || (*argvp)[1][1] == '\0'
          || (*argvp)[1][2] != '\0')
        { // (no - tag) or (only - tag) or (is long str/ not single letter)
          return result;
        }

      skip = false;
      for (int i = 0; i < nops && !skip; i++)
        {
          // find match, brute force
          // single letter, why bother ??
          if (!strncmp (&((*argvp)[1][1]), options[i].opt_name.c_str (),
                        options[i].opt_name.size ())
              && (strlen ((*argvp)[1]) - 1 == options[i].opt_name.size ()))
            {
              switch (options[i].type)
                {
                case T_NUM:
                  if (isdigit ((*argvp)[2][0]))
                    {
                      ++*argvp;
                      --*argcp;
                      *((uint32_t *)options[i].val) = atoi ((*argvp)[1]);
                    }
                  else
                    {
                      std::cerr << "Argument for parameter wrong. "
                                << std::endl;
                      return 0;
                    }
                  break;
                case T_DOUBLE:
                  ++*argvp;
                  --*argcp;
                  *((double *)options[i].val) = atof ((*argvp)[1]);
                  break;
                case T_STR:
                  ++*argvp;
                  --*argcp;
                  *((std::string *)options[i].val) = (*argvp)[1];
                  break;
                case T_FLAG:
                  *((bool *)options[i].val) = true;
                  break;
                }
              ++result;
              ++*argvp;
              --*argcp;
              options[i].set = true;
              skip = true;
            }
        }
      if (skip == false)
        {
          std::cerr << "Parameter not recognized. " << std::endl;
          return 0;
        }
    }

  for (int i = 0; i < nops; i++)
    {
      if (options[i].required && !options[i].set)
        return 0;
    }
  return 1;
}

void
print_usage (std::string progname, parsing_ctx *options, uint32_t nops)
{
  uint32_t i;
  std::cout << "Usage: " << progname << std::endl;
  for (i = 0; i < nops; i++)
    {
      std::cout << " -" << options[i].opt_name << " [" << options[i].help_str
                << (options[i].required ? ", required" : ", optional") << "]"
                << std::endl;
    }
  std::cout << std::endl << "Program exiting" << std::endl;
}
