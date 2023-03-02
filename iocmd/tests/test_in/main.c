#include "iocmd.h"
#include <stdio.h>
#include <string.h>

#define TEST_STRING     "test_string123"

static uint32_t test_id = 0;

static void test_cmd_test(IOCMD_Arg_DT *arg);
static void test_cmd_string(IOCMD_Arg_DT *arg);
static void test_cmd_group(IOCMD_Arg_DT *arg);
static void read_cmd_string(IOCMD_Arg_DT *arg);
static void read_cmd_u32(IOCMD_Arg_DT *arg);

static IOCMD_Command_Tree_XT cmd_tree[] =
{
   IOCMD_ELEM(       "root",       test_cmd_group,    "test elem on root\nnextline"),
   IOCMD_GROUP_START("test",                          "test command root\n\r"
                                                      "this command has multi-line descrpition\n"
                                                      "only to show that it is posible."),
   IOCMD_GROUP_START(   "group",                      "test group of commands.\r"
                                                      "Here we also put one more line."),
   IOCMD_ELEM(             "",     test_cmd_group,    "test elem"),
   IOCMD_GROUP_END(),
   {"wtype", "wrong type", NULL, 22},
   IOCMD_ELEM(          "string",  test_cmd_string,   "test elem to get string"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START("read",                          "reading group of commands\n"
                                                      "second line."),
   IOCMD_ELEM(          "string",  read_cmd_string,   "read elem to get string\n"
                                                      "Here we also put one more line."),
   IOCMD_ELEM(          "u32",     read_cmd_u32,      "read elem to get uint32_t\n"
                                                      "and here as well."),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START("read2",                         "reading group of commands"),
   IOCMD_ELEM(          "string2", read_cmd_string,   "read elem to get string"),
   IOCMD_ELEM(          "u322",    read_cmd_u32,      "read elem to get uint32_t"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START("write",                         "writing group of commands"),
   IOCMD_GROUP_END()
};

static IOCMD_Command_Tree_XT cmd_second_tree[] =
{
   IOCMD_GROUP_START("alt",                           "test command root\nnextline"),
   IOCMD_GROUP_START(   "test",                       "test command\n\r"
                                                      "this command has multi-line descrpition\n"
                                                      "only to show that it is posible."),
   IOCMD_GROUP_START(      "group",                   "test group of commands.\r"
                                                      "Here we also put one more line."),
   IOCMD_ELEM(                "",     test_cmd_group, "test elem"),
   IOCMD_GROUP_END(),
   {"wtype", "wrong type", NULL, 22},
   IOCMD_ELEM(             "string",  test_cmd_string,"test elem to get string"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(   "read",                       "reading group of commands\n"
                                                      "second line."),
   IOCMD_ELEM(             "string",  read_cmd_string,"read elem to get string\n"
                                                      "Here we also put one more line."),
   IOCMD_ELEM(             "u32",     read_cmd_u32,   "read elem to get uint32_t\n"
                                                      "and here as well."),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(   "read2",                      "reading group of commands"),
   IOCMD_ELEM(             "string2", read_cmd_string,"read elem to get string"),
   IOCMD_ELEM(             "u322",    read_cmd_u32,   "read elem to get uint32_t"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(   "write",                      "writing group of commands"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_END()
};

static char main_printf_buf[65536];
static int  main_printf_buf_pos = 0;

int main_printf(const char *format, ...)
{
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format))
   {
      va_start(arguments, format);

      result = vsnprintf(
         &main_printf_buf[main_printf_buf_pos],
         sizeof(main_printf_buf) - main_printf_buf_pos - 1,
         format,
         arguments);

      main_printf_buf_pos += result;

      main_printf_buf[main_printf_buf_pos] = 0;

      va_end(arguments);
   }

   return result;
}

static void test_cmd_string(IOCMD_Arg_DT *arg)
{
   test_id = 1;
}

static void test_cmd_group(IOCMD_Arg_DT *arg)
{
   test_id = 2;
}

static void read_cmd_string(IOCMD_Arg_DT *arg)
{
   char buffer[256];

   if((sizeof(TEST_STRING) - 1) == IOCMD_Arg_Get_String_Length(arg, NULL, NULL))
   {
      if((sizeof(TEST_STRING) - 1) == IOCMD_Arg_Get_String(arg, buffer, sizeof(buffer), NULL, NULL))
      {
         if(0 == strcmp(buffer, TEST_STRING))
         {
            test_id = 3;
         }
      }
   }
}

static void read_cmd_u32(IOCMD_Arg_DT *arg)
{
   test_id = 4;
}

static void test_test_string(void)
{
   const char *argv[] =
   {
      "test string"
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(1 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_test_unknown(void)
{
   const char *argv[] =
   {
      "test unknown"
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, "Command not found!" IOCMD_PRINT_ENDLINE IOCMD_PRINT_ENDLINE)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_test_wtype(void)
{
   const char *argv[] =
   {
      "test wtype"
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, "Command not found!" IOCMD_PRINT_ENDLINE IOCMD_PRINT_ENDLINE)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_group(void)
{
   const char *argv[] =
   {
      "test group"
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(2 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_read(void)
{
   const char *argv[] =
   {
      "read"
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, "Command not found!" IOCMD_PRINT_ENDLINE IOCMD_PRINT_ENDLINE)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_read_string(void)
{
   char string[512];
   const char *argv[] =
   {
      string
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   sprintf(string, "read string %s", TEST_STRING);

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(3 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_group_man(void)
{
   const char *argv[] =
   {
      "test group " IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"group\" - test group of commands."                                                        IOCMD_PRINT_ENDLINE
      "  *       Here we also put one more line."                                                  IOCMD_PRINT_ENDLINE
      "  |->\"\" - test elem"                                                                      IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_man(void)
{
   const char *argv[] =
   {
      "test " IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"test\"        - test command root"                                                        IOCMD_PRINT_ENDLINE
      "  *             this command has multi-line descrpition"                                    IOCMD_PRINT_ENDLINE
      "  *             only to show that it is posible."                                           IOCMD_PRINT_ENDLINE
      "  |->\"group\"  - test group of commands."                                                  IOCMD_PRINT_ENDLINE
      "  *    *        Here we also put one more line."                                            IOCMD_PRINT_ENDLINE
      "  *    |->\"\"  - test elem"                                                                IOCMD_PRINT_ENDLINE
      "  |->\"string\" - test elem to get string"                                                  IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void read_man(void)
{
   const char *argv[] =
   {
      "read " IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"read\"        - reading group of commands"                                                IOCMD_PRINT_ENDLINE
      "  *             second line."                                                               IOCMD_PRINT_ENDLINE
      "  |->\"string\" - read elem to get string"                                                  IOCMD_PRINT_ENDLINE
      "  *             Here we also put one more line."                                            IOCMD_PRINT_ENDLINE
      "  |->\"u32\"    - read elem to get uint32_t"                                                IOCMD_PRINT_ENDLINE
      "  *             and here as well."                                                          IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void write_man(void)
{
   const char *argv[] =
   {
      "write " IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"write\" - writing group of commands"                                                      IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

/*
  |->"test"           - test command root
  *    |                this command has multi-line descrpition
  *    |                only to show that it is posible.
  *    |->"group"     - test group of commands.
  *    *    |           Here we also put one more line.
  *    *    |->""     - test elem
  *    |->"wtype"     - wrong type
  *    |->"string"    - test elem to get string
  |->"read"           - reading group of commands
  *    |->"string"    - read elem to get string
  *    |->"u32"       - read elem to get uint32_t
  |->"read2"          - reading group of commands
  *    |->"string2"   - read elem to get string
  *    |->"u322"      - read elem to get uint32_t
  |->"write"          - writing group of commands

*/

static void man(void)
{
   const char *argv[] =
   {
      IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"root\"         - test elem on root"                                                       IOCMD_PRINT_ENDLINE
      "  *              nextline"                                                                  IOCMD_PRINT_ENDLINE
      "\"test\"         - test command root"                                                       IOCMD_PRINT_ENDLINE
      "  *              this command has multi-line descrpition"                                   IOCMD_PRINT_ENDLINE
      "  *              only to show that it is posible."                                          IOCMD_PRINT_ENDLINE
      "  |->\"group\"   - test group of commands."                                                 IOCMD_PRINT_ENDLINE
      "  *    *         Here we also put one more line."                                           IOCMD_PRINT_ENDLINE
      "  *    |->\"\"   - test elem"                                                               IOCMD_PRINT_ENDLINE
      "  |->\"string\"  - test elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "\"read\"         - reading group of commands"                                               IOCMD_PRINT_ENDLINE
      "  *              second line."                                                              IOCMD_PRINT_ENDLINE
      "  |->\"string\"  - read elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "  *              Here we also put one more line."                                           IOCMD_PRINT_ENDLINE
      "  |->\"u32\"     - read elem to get uint32_t"                                               IOCMD_PRINT_ENDLINE
      "  *              and here as well."                                                         IOCMD_PRINT_ENDLINE
      "\"read2\"        - reading group of commands"                                               IOCMD_PRINT_ENDLINE
      "  |->\"string2\" - read elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "  |->\"u322\"    - read elem to get uint32_t"                                               IOCMD_PRINT_ENDLINE
      "\"write\"        - writing group of commands"                                               IOCMD_PRINT_ENDLINE

                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void man_two_trees(void)
{
   const char *argv[] =
   {
      IOCMD_IN_MANUAL_PATTERN
   };
   const char printout[] =
   {
      "\"root\"         - test elem on root"                                                       IOCMD_PRINT_ENDLINE
      "  *              nextline"                                                                  IOCMD_PRINT_ENDLINE
      "\"test\"         - test command root"                                                       IOCMD_PRINT_ENDLINE
      "  *              this command has multi-line descrpition"                                   IOCMD_PRINT_ENDLINE
      "  *              only to show that it is posible."                                          IOCMD_PRINT_ENDLINE
      "  |->\"group\"   - test group of commands."                                                 IOCMD_PRINT_ENDLINE
      "  *    *         Here we also put one more line."                                           IOCMD_PRINT_ENDLINE
      "  *    |->\"\"   - test elem"                                                               IOCMD_PRINT_ENDLINE
      "  |->\"string\"  - test elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "\"read\"         - reading group of commands"                                               IOCMD_PRINT_ENDLINE
      "  *              second line."                                                              IOCMD_PRINT_ENDLINE
      "  |->\"string\"  - read elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "  *              Here we also put one more line."                                           IOCMD_PRINT_ENDLINE
      "  |->\"u32\"     - read elem to get uint32_t"                                               IOCMD_PRINT_ENDLINE
      "  *              and here as well."                                                         IOCMD_PRINT_ENDLINE
      "\"read2\"        - reading group of commands"                                               IOCMD_PRINT_ENDLINE
      "  |->\"string2\" - read elem to get string"                                                 IOCMD_PRINT_ENDLINE
      "  |->\"u322\"    - read elem to get uint32_t"                                               IOCMD_PRINT_ENDLINE
      "\"write\"        - writing group of commands"                                               IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"alt\"               - test command root"                                                  IOCMD_PRINT_ENDLINE
      "  *                   nextline"                                                             IOCMD_PRINT_ENDLINE
      "  |->\"test\"         - test command"                                                       IOCMD_PRINT_ENDLINE
      "  *    *              this command has multi-line descrpition"                              IOCMD_PRINT_ENDLINE
      "  *    *              only to show that it is posible."                                     IOCMD_PRINT_ENDLINE
      "  *    |->\"group\"   - test group of commands."                                            IOCMD_PRINT_ENDLINE
      "  *    *    *         Here we also put one more line."                                      IOCMD_PRINT_ENDLINE
      "  *    *    |->\"\"   - test elem"                                                          IOCMD_PRINT_ENDLINE
      "  *    |->\"string\"  - test elem to get string"                                            IOCMD_PRINT_ENDLINE
      "  |->\"read\"         - reading group of commands"                                          IOCMD_PRINT_ENDLINE
      "  *    *              second line."                                                         IOCMD_PRINT_ENDLINE
      "  *    |->\"string\"  - read elem to get string"                                            IOCMD_PRINT_ENDLINE
      "  *    *              Here we also put one more line."                                      IOCMD_PRINT_ENDLINE
      "  *    |->\"u32\"     - read elem to get uint32_t"                                          IOCMD_PRINT_ENDLINE
      "  *    *              and here as well."                                                    IOCMD_PRINT_ENDLINE
      "  |->\"read2\"        - reading group of commands"                                          IOCMD_PRINT_ENDLINE
      "  *    |->\"string2\" - read elem to get string"                                            IOCMD_PRINT_ENDLINE
      "  *    |->\"u322\"    - read elem to get uint32_t"                                          IOCMD_PRINT_ENDLINE
      "  |->\"write\"        - writing group of commands"                                          IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   if(IOCMD_BOOL_IS_FALSE(IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_FALSE)))
   {
      (void)IOCMD_Parse_Command(1, argv, NULL, cmd_second_tree, Num_Elems(cmd_second_tree), IOCMD_TRUE);
   }

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", "man two trees");
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_group_help(void)
{
   const char *argv[] =
   {
      "test group " IOCMD_IN_HELP_PATTERN
   };
   const char printout[] =
   {
      "\"group\" - test group of commands...."                                                     IOCMD_PRINT_ENDLINE
      "  |->\"\" - test elem"                                                                      IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
      "\"list\" - lists all commands in current catalogue only"                                    IOCMD_PRINT_ENDLINE
      "\"help\" - lists all commands in current catalogue and all sub-catalogues"                  IOCMD_PRINT_ENDLINE
      "\"man\"  - prints detailed manual for commands in current catalogue and all sub-catalogues" IOCMD_PRINT_ENDLINE
                                                                                                   IOCMD_PRINT_ENDLINE
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if((0 == test_id) && (0 == strcmp(main_printf_buf, printout)))
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_help(void)
{
   const char *argv[] =
   {
      "test " IOCMD_IN_HELP_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void read_help(void)
{
   const char *argv[] =
   {
      "read " IOCMD_IN_HELP_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void write_help(void)
{
   const char *argv[] =
   {
      "write " IOCMD_IN_HELP_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void help(void)
{
   const char *argv[] =
   {
      IOCMD_IN_HELP_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_group_list(void)
{
   const char *argv[] =
   {
      "test group " IOCMD_IN_LIST_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void test_list(void)
{
   const char *argv[] =
   {
      "test " IOCMD_IN_LIST_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void read_list(void)
{
   const char *argv[] =
   {
      "read " IOCMD_IN_LIST_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void write_list(void)
{
   const char *argv[] =
   {
      "write " IOCMD_IN_LIST_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

static void list(void)
{
   const char *argv[] =
   {
      IOCMD_IN_LIST_PATTERN
   };

   test_id = 0;
   main_printf_buf_pos = 0;

   IOCMD_Parse_Command(1, argv, NULL, cmd_tree, Num_Elems(cmd_tree), IOCMD_TRUE);

   if(0 == test_id)
   {
      printf("%-32s... OK\n\r", argv[0]);
      /* OK */
   }
   else
   {
      printf("error at %s; test_id: %d\n\r", __FUNCTION__, test_id);
   }
}

int main(void)
{
   test_test_string();
   test_test_unknown();
   test_test_wtype();
   test_group();
   test_read();
   test_read_string();
   test_group_man();
   test_man();
   read_man();
   write_man();
   man();
   man_two_trees();
   test_group_help();
   test_help();
   read_help();
   write_help();
   help();
   test_group_list();
   test_list();
   read_list();
   write_list();
   list();

   return 0;
}
