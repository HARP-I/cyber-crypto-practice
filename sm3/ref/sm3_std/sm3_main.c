#include "print.h"
#include "sm3_std.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
  uint32_t *message;
  uint32_t *expected;
  int length;
} test_case;

/**
 * @brief run test case
 * @return int
 *          0:    PASS
 *          else: FAIL
 */
int
sm3_run_example (test_case tc)
{
  unsigned char digest[32] = { 0 };

  debug_print ("-----------------------"
               " Message Input m Begin "
               "-------------------------\n");
  print_bytes ((unsigned *)tc.message, tc.length);
  debug_print ("----------------------- "
               "Message Input m End "
               "---------------------------\n");

  sm3 ((unsigned char *)tc.message, tc.length, (unsigned char *)digest);

  debug_print ("+++++++++++++++++++++++++++++++"
               " RESULT "
               "++++++++++++++++++++++++++++++++\n");
  debug_print ("RESULTS:\n");
  debug_print (" Expected:\n");
  print_hash ((unsigned *)tc.expected);

  debug_print (" Digest:\n");
  print_hash ((unsigned *)digest);

  return memcmp ((unsigned char *)digest, (unsigned char *)tc.expected, 32);
}

int
main (int argc, char **argv)
{
  /*
   * This test vector comes from Example 1 of GB/T 3290X-2016,
   * and described in Internet Draft draft-oscca-cfrg-sm3-XX.
   */

  int i;
  test_case tests[20] = { 0 };

  /* Example 1, From GB/T 32905-2016 */
  /* "abc" */
  static const uint32_t gbt32905m01[1] = { 0x00636261 };
  static const uint32_t gbt32905e01[8]
      = { 0x66c7f0f4, 0x62eeedd9, 0xd1f2d46b, 0xdc10e4e2,
          0x4167c487, 0x5cf2f7a2, 0x297da02b, 0x8f4ba8e0 };
  static const int gbt32905l01 = 3;
  test_case gbt32905t01
      = { (uint32_t *)&gbt32905m01, (uint32_t *)&gbt32905e01, gbt32905l01 };
  tests[0] = gbt32905t01;

  /* Example 2, From GB/T 32905-2016 */
  /*
   * "abcdabcdabcdabcdabcdabcdabcdabcd" +
   * "abcdabcdabcdabcdabcdabcdabcdabcd"
   */
  static const uint32_t gbt32905m02[16]
      = { 0x61626364, 0x61626364, 0x61626364, 0x61626364,
          0x61626364, 0x61626364, 0x61626364, 0x61626364,
          0x61626364, 0x61626364, 0x61626364, 0x61626364,
          0x61626364, 0x61626364, 0x61626364, 0x61626364 };
  static const uint32_t gbt32905e02[8]
      = { 0xdebe9ff9, 0x2275b8a1, 0x38604889, 0xc18e5a4d,
          0x6fdb70e5, 0x387e5765, 0x293dcba3, 0x9c0c5732 };
  static const int gbt32905l02 = 64;
  test_case gbt32905t02
      = { (uint32_t *)&gbt32905m02, (uint32_t *)&gbt32905e02, gbt32905l02 };
  tests[1] = gbt32905t02;

  /*
   * GB/T 32918.2-2016 A.2 Example 1
   */
  static const uint32_t gbt329182m01[53] = {
    0x0090414C, 0x49434531, 0x32334059, 0x41484F4F, 0x2E434F4D, 0x787968B4,
    0xFA32C3FD, 0x2417842E, 0x73BBFEFF, 0x2F3C848B, 0x6831D7E0, 0xEC65228B,
    0x3937E498, 0x63E4C6D3, 0xB23B0C84, 0x9CF84241, 0x484BFE48, 0xF61D59A5,
    0xB16BA06E, 0x6E12D1DA, 0x27C5249A, 0x421DEBD6, 0x1B62EAB6, 0x746434EB,
    0xC3CC315E, 0x32220B3B, 0xADD50BDC, 0x4C4E6C14, 0x7FEDD43D, 0x0680512B,
    0xCBB42C07, 0xD47349D2, 0x153B70C4, 0xE5D7FDFC, 0xBFA36EA1, 0xA85841B9,
    0xE46E09A2, 0x0AE4C779, 0x8AA0F119, 0x471BEE11, 0x825BE462, 0x02BB79E2,
    0xA5844495, 0xE97C04FF, 0x4DF2548A, 0x7C0240F8, 0x8F1CD4E1, 0x6352A73C,
    0x17B7F16F, 0x07353E53, 0xA176D684, 0xA9FE0C6B, 0xB798E857
  };
  static const uint32_t gbt329182e01[8]
      = { 0xF4A38489, 0xE32B45B6, 0xF876E3AC, 0x2168CA39,
          0x2362DC8F, 0x23459C1D, 0x1146FC3D, 0xBFB7BC9A };
  static const int gbt329182l01 = 212;
  test_case gbt329182t01
      = { (uint32_t *)&gbt329182m01, (uint32_t *)&gbt329182e01, gbt329182l01 };
  tests[2] = gbt329182t01;

  /*
   * GB/T 32918.2-2016 A.2 Example 2
   */
  static const uint32_t gbt329182m02[12]
      = { 0xF4A38489, 0xE32B45B6, 0xF876E3AC, 0x2168CA39,
          0x2362DC8F, 0x23459C1D, 0x1146FC3D, 0xBFB7BC9A,
          0x6D657373, 0x61676520, 0x64696765, 0x00007473 };
  static const uint32_t gbt329182e02[8]
      = { 0xB524F552, 0xCD82B8B0, 0x28476E00, 0x5C377FB1,
          0x9A87E6FC, 0x682D48BB, 0x5D42E3D9, 0xB9EFFE76 };
  static const int gbt329182l02 = 46;
  test_case gbt329182t02
      = { (uint32_t *)&gbt329182m02, (uint32_t *)&gbt329182e02, gbt329182l02 };
  tests[3] = gbt329182t02;

  /*
   * GB/T 32918.2-2016 A.3 Example 1
   */
  static const uint32_t gbt329182m03[55]
      = { 0x0090414C, 0x49434531, 0x32334059, 0x41484F4F, 0x2E434F4D,
          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
          0x00000000, 0x00000000, 0x00000000, 0x0000E78B, 0xCD09746C,
          0x202378A7, 0xE72B12BC, 0xE00266B9, 0x627ECB0B, 0x5A25367A,
          0xD1AD4CC6, 0x242B00CD, 0xB9CA7F1E, 0x6B0441F6, 0x58343F4B,
          0x10297C0E, 0xF9B64910, 0x82400A62, 0xE7A74857, 0x35FADD01,
          0x3DE74DA6, 0x5951C4D7, 0x6DC89220, 0xD5F7777A, 0x611B1C38,
          0xBAE260B1, 0x75951DC8, 0x060C2B3E, 0x01659616, 0x45281A86,
          0x26607B91, 0x7F657D7E, 0x9382F1EA, 0x5CD931F4, 0x0F6627F3,
          0x57542653, 0xB2016865, 0x22130D59, 0x0FB8DE63, 0x5D8FCA71,
          0x5CC6BF3D, 0x05BEF3F7, 0x5DA5D543, 0x45444816, 0x00001266 };
  static const uint32_t gbt329182e03[8]
      = { 0x26352AF8, 0x2EC19F20, 0x7BBC6F94, 0x74E11E90,
          0xCE0F7DDA, 0xCE03B27F, 0x801817E8, 0x97A81FD5 };
  static const int gbt329182l03 = 218;
  test_case gbt329182t03
      = { (uint32_t *)&gbt329182m03, (uint32_t *)&gbt329182e03, gbt329182l03 };
  tests[4] = gbt329182t03;

  /*
   * GB/T 32918.2-2016 A.3 Example 2
   */
  static const uint32_t gbt329182m04[12]
      = { 0x26352AF8, 0x2EC19F20, 0x7BBC6F94, 0x74E11E90,
          0xCE0F7DDA, 0xCE03B27F, 0x801817E8, 0x97A81FD5,
          0x6D657373, 0x61676520, 0x64696765, 0x00007473 };
  static const uint32_t gbt329182e04[8]
      = { 0xAD673CBD, 0xA3114171, 0x29A9EAA5, 0xF9AB1AA1,
          0x633AD477, 0x18A84DFD, 0x46C17C6F, 0xA0AA3B12 };
  static const int gbt329182l04 = 46;
  test_case gbt329182t04
      = { (uint32_t *)&gbt329182m04, (uint32_t *)&gbt329182e04, gbt329182l04 };
  tests[5] = gbt329182t04;

  /*
   * GB/T 32918.3-2016 A.2 Example 1
   */
  static const uint32_t gbt329183m01[53] = {
    0x0090414C, 0x49434531, 0x32334059, 0x41484F4F, 0x2E434F4D, 0x787968B4,
    0xFA32C3FD, 0x2417842E, 0x73BBFEFF, 0x2F3C848B, 0x6831D7E0, 0xEC65228B,
    0x3937E498, 0x63E4C6D3, 0xB23B0C84, 0x9CF84241, 0x484BFE48, 0xF61D59A5,
    0xB16BA06E, 0x6E12D1DA, 0x27C5249A, 0x421DEBD6, 0x1B62EAB6, 0x746434EB,
    0xC3CC315E, 0x32220B3B, 0xADD50BDC, 0x4C4E6C14, 0x7FEDD43D, 0x0680512B,
    0xCBB42C07, 0xD47349D2, 0x153B70C4, 0xE5D7FDFC, 0xBFA36EA1, 0xA85841B9,
    0xE46E09A2, 0x3099093B, 0xF3C137D8, 0xFCBBCDF4, 0xA2AE50F3, 0xB0F216C3,
    0x122D7942, 0x5FE03A45, 0xDBFE1655, 0x3DF79E8D, 0xAC1CF0EC, 0xBAA2F2B4,
    0x9D51A4B3, 0x87F2EFAF, 0x48233908, 0x6A27A8E0, 0x5BAED98B
  };
  static const uint32_t gbt329183e01[8]
      = { 0xE4D1D0C3, 0xCA4C7F11, 0xBC8FF8CB, 0x3F4C02A7,
          0x8F108FA0, 0x98E51A66, 0x8487240F, 0x75E20F31 };
  static const int gbt329183l01 = 212;
  test_case gbt329183t01
      = { (uint32_t *)&gbt329183m01, (uint32_t *)&gbt329183e01, gbt329183l01 };
  tests[6] = gbt329183t01;

  /*
   * GB/T 32918.3-2016 A.2 Example 2
   */
  static const uint32_t gbt329183m02[53] = {
    0x00884249, 0x4C4C3435, 0x36405941, 0x484F4F2E, 0x434F4D78, 0x7968B4FA,
    0x32C3FD24, 0x17842E73, 0xBBFEFF2F, 0x3C848B68, 0x31D7E0EC, 0x65228B39,
    0x37E49863, 0xE4C6D3B2, 0x3B0C849C, 0xF8424148, 0x4BFE48F6, 0x1D59A5B1,
    0x6BA06E6E, 0x12D1DA27, 0xC5249A42, 0x1DEBD61B, 0x62EAB674, 0x6434EBC3,
    0xCC315E32, 0x220B3BAD, 0xD50BDC4C, 0x4E6C147F, 0xEDD43D06, 0x80512BCB,
    0xB42C07D4, 0x7349D215, 0x3B70C4E5, 0xD7FDFCBF, 0xA36EA1A8, 0x5841B9E4,
    0x6E09A224, 0x5493D446, 0xC38D8CC0, 0xF1183746, 0x90E7DF63, 0x3A8A4BFB,
    0x3329B5EC, 0xE604B2B4, 0xF37F4353, 0xC0869F4B, 0x9E17773D, 0xE68FEC45,
    0xE14904E0, 0xDEA45BF6, 0xCECF9918, 0xC85EA047, 0x004C0AC6
  };
  static const uint32_t gbt329183e02[8]
      = { 0x6B4B6D0E, 0x276691BD, 0x4A11BF72, 0xF4FB501A,
          0xE309FDAC, 0xB72FA6CC, 0x336E6656, 0x119ABD67 };
  static const int gbt329183l02 = 211;
  test_case gbt329183t02
      = { (uint32_t *)&gbt329183m02, (uint32_t *)&gbt329183e02, gbt329183l02 };
  tests[7] = gbt329183t02;

  /*
   * GB/T 32918.3-2016 A.2 Example 3
   */
  static const uint32_t gbt329183m03[56] = {
    0x47C82653, 0x4DC2F6F1, 0xFBF28728, 0xDD658F21, 0xE174F481, 0x79ACEF29,
    0x00F8B7F5, 0x66E40905, 0xE4D1D0C3, 0xCA4C7F11, 0xBC8FF8CB, 0x3F4C02A7,
    0x8F108FA0, 0x98E51A66, 0x8487240F, 0x75E20F31, 0x6B4B6D0E, 0x276691BD,
    0x4A11BF72, 0xF4FB501A, 0xE309FDAC, 0xB72FA6CC, 0x336E6656, 0x119ABD67,
    0x6CB56338, 0x16F4DD56, 0x0B1DEC45, 0x8310CBCC, 0x6856C095, 0x05324A6D,
    0x23150C40, 0x8F162BF0, 0x0D6FCF62, 0xF1036C0A, 0x1B6DACCF, 0x57399223,
    0xA65F7D7B, 0xF2D9637E, 0x5BBBEB85, 0x7961BF1A, 0x1799B2A2, 0xC7782953,
    0x00D9A232, 0x5C686129, 0xB8F2B533, 0x7B3DCF45, 0x14E8BBC1, 0x9D900EE5,
    0x54C9288C, 0x82733EFD, 0xF7808AE7, 0xF27D0E73, 0x2F7C73A7, 0xD9AC98B7,
    0xD8740A91, 0xD0DB3CF4
  };
  static const uint32_t gbt329183e03[8]
      = { 0xFF49D95B, 0xD45FCE99, 0xED54A8AD, 0x7A709110,
          0x9F513944, 0x42916BD1, 0x54D1DE43, 0x79D97647 };
  static const int gbt329183l03 = 224;
  test_case gbt329183t03
      = { (uint32_t *)&gbt329183m03, (uint32_t *)&gbt329183e03, gbt329183l03 };
  tests[8] = gbt329183t03;

  /*
   * GB/T 32918.3-2016 A.2 Example 4
   */
  static const uint32_t gbt329183m04[17]
      = { 0x022AF86E, 0xFE732CF1, 0x2AD0E09A, 0x1F2556CC, 0x650D9CCC,
          0xE3E24986, 0x6BBB5C68, 0x46A4C4A2, 0x95FF49D9, 0x5BD45FCE,
          0x99ED54A8, 0xAD7A7091, 0x109F5139, 0x4442916B, 0xD154D1DE,
          0x4379D976, 0x00000047 };
  static const uint32_t gbt329183e04[8]
      = { 0x284C8F19, 0x8F141B50, 0x2E81250F, 0x1581C7E9,
          0xEEB4CA69, 0x90F9E02D, 0xF388B454, 0x71F5BC5C };
  static const int gbt329183l04 = 65;
  test_case gbt329183t04
      = { (uint32_t *)&gbt329183m04, (uint32_t *)&gbt329183e04, gbt329183l04 };
  tests[9] = gbt329183t04;

  /*
   * GB/T 32918.3-2016 A.2 Example 5
   */
  static const uint32_t gbt329183m05[17]
      = { 0x032AF86E, 0xFE732CF1, 0x2AD0E09A, 0x1F2556CC, 0x650D9CCC,
          0xE3E24986, 0x6BBB5C68, 0x46A4C4A2, 0x95FF49D9, 0x5BD45FCE,
          0x99ED54A8, 0xAD7A7091, 0x109F5139, 0x4442916B, 0xD154D1DE,
          0x4379D976, 0x00000047 };
  static const uint32_t gbt329183e05[8]
      = { 0x23444DAF, 0x8ED75343, 0x66CB901C, 0x84B3BDBB,
          0x63504F40, 0x65C1116C, 0x91A4C006, 0x97E6CF7A };
  static const int gbt329183l05 = 65;
  test_case gbt329183t05
      = { (uint32_t *)&gbt329183m05, (uint32_t *)&gbt329183e05, gbt329183l05 };
  tests[10] = gbt329183t05;

  /*
   * GB/T 32918.3-2016 A.3 Example 2
   */
  static const uint32_t gbt329183m07[55]
      = { 0x00884249, 0x4C4C3435, 0x36405941, 0x484F4F2E, 0x434F4D00,
          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
          0x00000000, 0x00000000, 0x00000000, 0x00E78BCD, 0x09746C20,
          0x2378A7E7, 0x2B12BCE0, 0x0266B962, 0x7ECB0B5A, 0x25367AD1,
          0xAD4CC624, 0x2B00CDB9, 0xCA7F1E6B, 0x0441F658, 0x343F4B10,
          0x297C0EF9, 0xB6491082, 0x400A62E7, 0xA7485735, 0xFADD013D,
          0xE74DA659, 0x51C4D76D, 0xC89220D5, 0xF7777A61, 0x1B1C38BA,
          0xE260B175, 0x951DC806, 0x0C2B3E00, 0x34297DD8, 0x3AB14D5B,
          0x393B6712, 0xF32B2F2E, 0x938D4690, 0xB095424B, 0x89DA880C,
          0x52D4A7D9, 0x0199BBF1, 0x1AC95A0E, 0xA34BBD00, 0xCA50B93E,
          0xC24ACB68, 0x335D20BA, 0x5DCFE3B3, 0x3BDBD2B6, 0x0000002D };
  static const uint32_t gbt329183e07[8]
      = { 0x557BAD30, 0xE183559A, 0xEEC3B225, 0x6E1C7C11,
          0xF870D22B, 0x165D015A, 0xCF9465B0, 0x9B87B527 };
  static const int gbt329183l07 = 217;
  test_case gbt329183t07
      = { (uint32_t *)&gbt329183m07, (uint32_t *)&gbt329183e07, gbt329183l07 };
  tests[12] = gbt329183t07;

  /*
   * GB/T 32918.3-2016 A.3 Example 3
   */
  static const uint32_t gbt329183m08[58] = {
    0x00DADD08, 0x7406221D, 0x657BC3FA, 0x79FF329B, 0xB022E9CB, 0x7DDFCFCC,
    0xFE277BE8, 0xCD4AE9B9, 0x54ECF008, 0x0215977B, 0x2E5D6D61, 0xB98A9944,
    0x2F03E880, 0x3DC39E34, 0x9F8DCA56, 0x21A9ACDF, 0x2B557BAD, 0x30E18355,
    0x9AEEC3B2, 0x256E1C7C, 0x11F870D2, 0x2B165D01, 0x5ACF9465, 0xB09B87B5,
    0x27018107, 0x6543ED19, 0x058C38B3, 0x13D73992, 0x1D46B800, 0x94D961A1,
    0x3673D4A5, 0xCF8C7159, 0xE30401D8, 0xCFFF7CA2, 0x7A01A2E8, 0x8C186737,
    0x48FDE9A7, 0x4C1F9B45, 0x646ECA09, 0x97293C15, 0xC34DD800, 0x2A4832B4,
    0xDCD399BA, 0xAB3FFFE7, 0xDD6CE6ED, 0x68CC43FF, 0xA5F2623B, 0x9BD04E46,
    0x8D322A2A, 0x0016599B, 0xB52ED9EA, 0xFAD01CFA, 0x453CF305, 0x2ED60184,
    0xD2EECFD4, 0x2B52DB74, 0x110B984C, 0x00000023
  };
  static const uint32_t gbt329183e08[8]
      = { 0xE05FE287, 0xB73B0CE6, 0x639524CD, 0x86694311,
          0x562914F4, 0xF6A34241, 0x01D885F8, 0x8B05369C };
  static const int gbt329183l08 = 229;
  test_case gbt329183t08
      = { (uint32_t *)&gbt329183m08, (uint32_t *)&gbt329183e08, gbt329183l08 };
  tests[13] = gbt329183t08;

  /*
   * GB/T 32918.3-2016 A.3 Example 4
   */
  static const uint32_t gbt329183m09[17]
      = { 0x0201F046, 0x4B1E8168, 0x4E5ED6EF, 0x281B5562, 0x4EF46CAA,
          0x3B2D3748, 0x4372D916, 0x10B69825, 0x2CC9E05F, 0xE287B73B,
          0x0CE66395, 0x24CD8669, 0x43115629, 0x14F4F6A3, 0x424101D8,
          0x85F88B05, 0x00009C36 };
  static const uint32_t gbt329183e09[8]
      = { 0x4EB47D28, 0xAD3906D6, 0x244D01E0, 0xF6AEC73B,
          0x0B51DE15, 0x74C13798, 0x184E4833, 0xDBAE295A };
  static const int gbt329183l09 = 66;
  test_case gbt329183t09
      = { (uint32_t *)&gbt329183m09, (uint32_t *)&gbt329183e09, gbt329183l09 };
  tests[14] = gbt329183t09;

  /*
   * GB/T 32918.3-2016 A.3 Example 5
   */
  static const uint32_t gbt329183m10[17]
      = { 0x0301F046, 0x4B1E8168, 0x4E5ED6EF, 0x281B5562, 0x4EF46CAA,
          0x3B2D3748, 0x4372D916, 0x10B69825, 0x2CC9E05F, 0xE287B73B,
          0x0CE66395, 0x24CD8669, 0x43115629, 0x14F4F6A3, 0x424101D8,
          0x85F88B05, 0x00009C36 };
  static const uint32_t gbt329183e10[8]
      = { 0x588AA670, 0x64F24DC2, 0x7CCAA1FA, 0xB7E27DFF,
          0x811D500A, 0xD7EF2FB8, 0xF69DDF48, 0xCC0FECB7 };
  static const int gbt329183l10 = 66;
  test_case gbt329183t10
      = { (uint32_t *)&gbt329183m10, (uint32_t *)&gbt329183e10, gbt329183l10 };
  tests[15] = gbt329183t10;

  /*
   * GB/T 32918.4-2016 A.2 Example 1
   */
  static const uint32_t gbt329184m01[17]
      = { 0x57E7B636, 0x23FAE5F0, 0x8CDA468E, 0x872A20AF, 0xA03DED41,
          0xBF140377, 0x656E6372, 0x79707469, 0x6F6E2073, 0x74616E64,
          0x6172640E, 0x040DC83A, 0xF31A6799, 0x1F2B01EB, 0xF9EFD888,
          0x1F0A0493, 0x00030600 };
  static const uint32_t gbt329184e01[8]
      = { 0x6AFB3BCE, 0xBD76F82B, 0x252CE5EB, 0x25B57996,
          0x86902B8C, 0xF2FD8753, 0x6E55EF76, 0x03B09E7C };
  static const int gbt329184l01 = 67;
  test_case gbt329184t01
      = { (uint32_t *)&gbt329184m01, (uint32_t *)&gbt329184e01, gbt329184l01 };
  tests[16] = gbt329184t01;

  /*
   * GB/T 32918.4-2016 A.2 Example 2
   */
  static const uint32_t gbt329184m02[21]
      = { 0x64D20D27, 0xD0632957, 0xF8028C1E, 0x024F6B02, 0xEDF23102,
          0xA566C932, 0xAE8BD613, 0xA8E865FE, 0x656E6372, 0x79707469,
          0x6F6E2073, 0x74616E64, 0x61726458, 0xD225ECA7, 0x84AE300A,
          0x81A2D482, 0x81A828E1, 0xCEDF11C4, 0x21909984, 0x02653750,
          0x0078BF77 };
  static const uint32_t gbt329184e02[8]
      = { 0x9C3D7360, 0xC30156FA, 0xB7C80A02, 0x76712DA9,
          0xD8094A63, 0x4B766D3A, 0x285E0748, 0x0653426D };
  static const int gbt329184l02 = 83;
  test_case gbt329184t02
      = { (uint32_t *)&gbt329184m02, (uint32_t *)&gbt329184e02, gbt329184l02 };
  tests[17] = gbt329184t02;

  /*
   * GB/T 32918.4-2016 A.3 Example 1
   */
  static const uint32_t gbt329184m03[18]
      = { 0x01C6271B, 0x31F6BE39, 0x6A4166C0, 0x616CF4A8, 0xACDA5BEF,
          0x4DCBF2DD, 0x42656E63, 0x72797074, 0x696F6E20, 0x7374616E,
          0x64617264, 0x0147AF35, 0xDFA1BFE2, 0xF161521B, 0xCF59BAB8,
          0x3564868D, 0x92958817, 0x00000035 };
  static const uint32_t gbt329184e03[8]
      = { 0xF0A41F6F, 0x48AC723C, 0xECFC4B76, 0x7299A5E2,
          0x5C064167, 0x9FBD2D4D, 0x20E9FFD5, 0xB9F0DAB8 };
  static const int gbt329184l03 = 69;
  test_case gbt329184t03
      = { (uint32_t *)&gbt329184m03, (uint32_t *)&gbt329184e03, gbt329184l03 };
  tests[18] = gbt329184t03;

  /*
   * GB/T 32918.4-2016 A.3 Example 2
   */
  static const uint32_t gbt329184m04[22]
      = { 0x0083E628, 0xCF701EE3, 0x141E8873, 0xFE55936A, 0xDF24963F,
          0x5DC9C648, 0x0566C80F, 0x8A1D8CC5, 0x1B656E63, 0x72797074,
          0x696F6E20, 0x7374616E, 0x64617264, 0x01524C64, 0x7F0C0412,
          0xDEFD468B, 0xDA3AE0E5, 0xA80FCC8F, 0x5C990FEE, 0x11602929,
          0x232DCD9F, 0x00000036 };
  static const uint32_t gbt329184e04[8]
      = { 0x73A48625, 0xD3758FA3, 0x7B3EAB80, 0xE9CFCABA,
          0x665E3199, 0xEA15A1FA, 0x8189D96F, 0x579125E4 };
  static const int gbt329184l04 = 85;
  test_case gbt329184t04
      = { (uint32_t *)&gbt329184m04, (uint32_t *)&gbt329184e04, gbt329184l04 };
  tests[19] = gbt329184t04;

  for (i = 0; i < 20; i++)
    {
      if (i == 11)
        continue;

      debug_print ("\n\n= Running Example[%i]\n", i);
      printf ("sm3_example[%2i]: %s\n", i,
              sm3_run_example (tests[i]) ? "FAIL" : "PASS");
      // printf("\n\n\n\n\n\n\n\n\n\n");
      debug_print ("\n\n= Running Example[%i] Done\n", i);
    }

  return 0;
}