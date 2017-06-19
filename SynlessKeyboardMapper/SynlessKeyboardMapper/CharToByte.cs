using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SynlessKeyboardMapper
{
    public static class CharToByte
    {
        public static Dictionary<string, byte> conv = new Dictionary<string, byte> ()
        {
            {"ESCAPE",0x01},
            {"F1",0xC2},
            {"F2",0xC3},
            {"F3",0xC4},
            {"F4",0xC5},
            {"F5",0xC6},
            {"F6",0xC7},
            {"F7",0xC8},
            {"F8",0xC9},
            {"F9",0xCA},

        };
    }
}
