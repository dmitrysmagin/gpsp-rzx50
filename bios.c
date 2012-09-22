/* BIOS HLE code
 * Adapted from VisualBoyAdvance source code.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include "common.h"

static s16 b_sinetable[256] = {
	(s16)0x0000, (s16)0x0192, (s16)0x0323, (s16)0x04B5, (s16)0x0645, (s16)0x07D5, (s16)0x0964, (s16)0x0AF1,
	(s16)0x0C7C, (s16)0x0E05, (s16)0x0F8C, (s16)0x1111, (s16)0x1294, (s16)0x1413, (s16)0x158F, (s16)0x1708,
	(s16)0x187D, (s16)0x19EF, (s16)0x1B5D, (s16)0x1CC6, (s16)0x1E2B, (s16)0x1F8B, (s16)0x20E7, (s16)0x223D,
	(s16)0x238E, (s16)0x24DA, (s16)0x261F, (s16)0x275F, (s16)0x2899, (s16)0x29CD, (s16)0x2AFA, (s16)0x2C21,
	(s16)0x2D41, (s16)0x2E5A, (s16)0x2F6B, (s16)0x3076, (s16)0x3179, (s16)0x3274, (s16)0x3367, (s16)0x3453,
	(s16)0x3536, (s16)0x3612, (s16)0x36E5, (s16)0x37AF, (s16)0x3871, (s16)0x392A, (s16)0x39DA, (s16)0x3A82,
	(s16)0x3B20, (s16)0x3BB6, (s16)0x3C42, (s16)0x3CC5, (s16)0x3D3E, (s16)0x3DAE, (s16)0x3E14, (s16)0x3E71,
	(s16)0x3EC5, (s16)0x3F0E, (s16)0x3F4E, (s16)0x3F84, (s16)0x3FB1, (s16)0x3FD3, (s16)0x3FEC, (s16)0x3FFB,
	(s16)0x4000, (s16)0x3FFB, (s16)0x3FEC, (s16)0x3FD3, (s16)0x3FB1, (s16)0x3F84, (s16)0x3F4E, (s16)0x3F0E,
	(s16)0x3EC5, (s16)0x3E71, (s16)0x3E14, (s16)0x3DAE, (s16)0x3D3E, (s16)0x3CC5, (s16)0x3C42, (s16)0x3BB6,
	(s16)0x3B20, (s16)0x3A82, (s16)0x39DA, (s16)0x392A, (s16)0x3871, (s16)0x37AF, (s16)0x36E5, (s16)0x3612,
	(s16)0x3536, (s16)0x3453, (s16)0x3367, (s16)0x3274, (s16)0x3179, (s16)0x3076, (s16)0x2F6B, (s16)0x2E5A,
	(s16)0x2D41, (s16)0x2C21, (s16)0x2AFA, (s16)0x29CD, (s16)0x2899, (s16)0x275F, (s16)0x261F, (s16)0x24DA,
	(s16)0x238E, (s16)0x223D, (s16)0x20E7, (s16)0x1F8B, (s16)0x1E2B, (s16)0x1CC6, (s16)0x1B5D, (s16)0x19EF,
	(s16)0x187D, (s16)0x1708, (s16)0x158F, (s16)0x1413, (s16)0x1294, (s16)0x1111, (s16)0x0F8C, (s16)0x0E05,
	(s16)0x0C7C, (s16)0x0AF1, (s16)0x0964, (s16)0x07D5, (s16)0x0645, (s16)0x04B5, (s16)0x0323, (s16)0x0192,
	(s16)0x0000, (s16)0xFE6E, (s16)0xFCDD, (s16)0xFB4B, (s16)0xF9BB, (s16)0xF82B, (s16)0xF69C, (s16)0xF50F,
	(s16)0xF384, (s16)0xF1FB, (s16)0xF074, (s16)0xEEEF, (s16)0xED6C, (s16)0xEBED, (s16)0xEA71, (s16)0xE8F8,
	(s16)0xE783, (s16)0xE611, (s16)0xE4A3, (s16)0xE33A, (s16)0xE1D5, (s16)0xE075, (s16)0xDF19, (s16)0xDDC3,
	(s16)0xDC72, (s16)0xDB26, (s16)0xD9E1, (s16)0xD8A1, (s16)0xD767, (s16)0xD633, (s16)0xD506, (s16)0xD3DF,
	(s16)0xD2BF, (s16)0xD1A6, (s16)0xD095, (s16)0xCF8A, (s16)0xCE87, (s16)0xCD8C, (s16)0xCC99, (s16)0xCBAD,
	(s16)0xCACA, (s16)0xC9EE, (s16)0xC91B, (s16)0xC851, (s16)0xC78F, (s16)0xC6D6, (s16)0xC626, (s16)0xC57E,
	(s16)0xC4E0, (s16)0xC44A, (s16)0xC3BE, (s16)0xC33B, (s16)0xC2C2, (s16)0xC252, (s16)0xC1EC, (s16)0xC18F,
	(s16)0xC13B, (s16)0xC0F2, (s16)0xC0B2, (s16)0xC07C, (s16)0xC04F, (s16)0xC02D, (s16)0xC014, (s16)0xC005,
	(s16)0xC000, (s16)0xC005, (s16)0xC014, (s16)0xC02D, (s16)0xC04F, (s16)0xC07C, (s16)0xC0B2, (s16)0xC0F2,
	(s16)0xC13B, (s16)0xC18F, (s16)0xC1EC, (s16)0xC252, (s16)0xC2C2, (s16)0xC33B, (s16)0xC3BE, (s16)0xC44A,
	(s16)0xC4E0, (s16)0xC57E, (s16)0xC626, (s16)0xC6D6, (s16)0xC78F, (s16)0xC851, (s16)0xC91B, (s16)0xC9EE,
	(s16)0xCACA, (s16)0xCBAD, (s16)0xCC99, (s16)0xCD8C, (s16)0xCE87, (s16)0xCF8A, (s16)0xD095, (s16)0xD1A6,
	(s16)0xD2BF, (s16)0xD3DF, (s16)0xD506, (s16)0xD633, (s16)0xD767, (s16)0xD8A1, (s16)0xD9E1, (s16)0xDB26,
	(s16)0xDC72, (s16)0xDDC3, (s16)0xDF19, (s16)0xE075, (s16)0xE1D5, (s16)0xE33A, (s16)0xE4A3, (s16)0xE611,
	(s16)0xE783, (s16)0xE8F8, (s16)0xEA71, (s16)0xEBED, (s16)0xED6C, (s16)0xEEEF, (s16)0xF074, (s16)0xF1FB,
	(s16)0xF384, (s16)0xF50F, (s16)0xF69C, (s16)0xF82B, (s16)0xF9BB, (s16)0xFB4B, (s16)0xFCDD, (s16)0xFE6E,
};

static u32 bios_sqrt(u32 value)
{
	return (u32) sqrt((double)value);
}

static u32 bios_arctan(u32 value)
{
	s32 a =  -(((s32)(value * value)) >> 14);
	s32 b = ((0xA9 * a) >> 14) + 0x390;
	b = ((b * a) >> 14) + 0x91C;
	b = ((b * a) >> 14) + 0xFB6;
	b = ((b * a) >> 14) + 0x16AA;
	b = ((b * a) >> 14) + 0x2081;
	b = ((b * a) >> 14) + 0x3651;
	b = ((b * a) >> 14) + 0xA2F9;
	a = ((s32)value * b) >> 16;
	return a;
}

static u32 bios_arctan2(s32 x, s32 y)
{
	if (y == 0)
	  return (x>>16) & 0x8000;

	if (x == 0)
	  return ((y>>16) & 0x8000) + 0x4000;

	if ((abs(x) > abs(y)) || ((abs(x) == abs(y)) && (!((x<0) && (y<0))))) {
		if (x < 0)
		  return bios_arctan((y << 14) / x) + 0x8000;
		return bios_arctan((y << 14) / x) + (((y >> 16) & 0x8000) << 1);
	}

	return (0x4000 + ((y >> 16) & 0x8000)) - bios_arctan((x << 14) / y);
}

static void bios_cpuset(u32 src, u32 dst, u32 cnt)
{
	if(((src & 0xe000000) != 0) &&
				((src + (((cnt << 11) >> 9) & 0x1FFFFF)) & 0xe000000) != 0) {
		u32 count = cnt & 0x1FFFFF;

		// 32-bit ?
		if((cnt >> 26) & 1) {
			// needed for 32-bit mode!
			src &= 0x0FFFFFFC;
			dst &= 0x0FFFFFFC;

			// fill ?
			if((cnt >> 24) & 1) {
				u32 value = read_memory32(src);
				while(count) {
					write_memory32(dst, value);
					dst += 4;
					count--;
				}
			} else {
				// copy
				while(count) {
					write_memory32(dst, read_memory32(src));
					src += 4;
					dst += 4;
					count--;
				}
			}
		} else {
			// 16-bit fill?
			if((cnt >> 24) & 1) {
				u16 value = read_memory16(src);
				while(count) {
					write_memory16(dst, value);
					dst += 2;
					count--;
				}
			} else {
				// copy
				while(count) {
					write_memory16(dst, read_memory16(src));
					src += 2;
					dst += 2;
					count--;
				}
			}
		}
	}
}

static void *get_address(u32 addr)
{
	switch(addr >> 24) {
		/* BIOS */
		case 0x00:
			if (reg[REG_PC] >= 0x4000)
				return (void *) bios_read_protect + (addr & 3);
			return (void *) bios_rom + (addr & 0x3fff);

		/* External work RAM */
		case 0x02:
			return (void *) ewram + (addr & 0x7fff) + ((addr & 0x38000) << 1) + 0x8000;

		/* Internal work RAM */
		case 0x03:
			return (void *) iwram + (addr & 0x7fff) + 0x8000;

		/* VRAM */
		case 0x06:
			addr &= 0x1ffff;
			if (addr >= 0x18000)
				addr -= 0x8000;
			return (void *) vram + addr;

		/* OAM RAM */
		case 0x07:
			oam_update = 1;
			return (void *) oam_ram + (addr & 0x3ff);

		/* Gamepak ROM */
		case 0x08 ... 0x0D:
			if ((addr & 0x1ffffff) >= gamepak_size)
				return NULL;

			u8 *map = memory_map_read[addr >> 15];
			if (!map)
				map = load_gamepak_page((addr >> 15) & 0x3ff);
			return (void *) map + (addr & 0x7fff);

		default:
			return NULL;
	}
}

static void bios_cpufastset(u32 src, u32 dst, u32 cnt)
{
	void *dstp;
	u32 count = cnt & 0x1FFFFF;

	if (count & 7)
		count = (count & ~7) + 8;

	if(((src & 0xe000000) == 0) ||
				((src + (((cnt << 11) >> 9) & 0x1FFFFF)) & 0xe000000) == 0)
		return;

	// needed for 32-bit mode!
	src &= 0x0FFFFFFC;
	dst &= 0x0FFFFFFC;

	dstp = get_address(dst);

	if((cnt >> 24) & 1) {
		// fill

		u32 value = read_memory32(src);
		int unexpected = value && (((value & 0xff) != ((value >> 8) & 0xff))
					|| ((value & 0xff) != ((value >> 16) & 0xff))
					|| ((value & 0xff) != ((value >> 24) & 0xff)));

		if (dstp && !unexpected) {
			memset(dstp, value, count * 4);
			return;
		}

		while(count > 0) {
			write_memory32(dst, value);
			dst += 4;
			count--;
		}

	} else {
		// copy

		if (dstp) {
			void *srcp = get_address(src);

			if (srcp) {
				memcpy(dstp, srcp, count * 4);
				return;
			}
		}

		while(count > 0) {
			write_memory32(dst, read_memory32(src));
			src += 4;
			dst += 4;
			count--;
		}
	}
}

static void bios_bgaffineset(u32 b_src, u32 dst, u32 b_num)
{
	u32 b_i;
	for(b_i = 0; b_i < b_num; b_i++) {
		s32 b_cx = read_memory32(b_src);
		b_src+=4;
		s32 b_cy = read_memory32(b_src);
		b_src+=4;
		s16 b_dispx = read_memory16(b_src);
		b_src+=2;
		s16 b_dispy = read_memory16(b_src);
		b_src+=2;
		s16 b_rx = read_memory16(b_src);
		b_src+=2;
		s16 b_ry = read_memory16(b_src);
		b_src+=2;
		u16 b_theta = read_memory16(b_src)>>8;
		b_src+=4; // keep structure alignment
		s32 b_a = b_sinetable[(b_theta+0x40)&255];
		s32 b_b = b_sinetable[b_theta];

		s16 b_dx =  (b_rx * b_a)>>14;
		s16 b_dmx = (b_rx * b_b)>>14;
		s16 b_dy =  (b_ry * b_b)>>14;
		s16 b_dmy = (b_ry * b_a)>>14;

		write_memory16(dst, b_dx);
		dst += 2;
		write_memory16(dst, -b_dmx);
		dst += 2;
		write_memory16(dst, b_dy);
		dst += 2;
		write_memory16(dst, b_dmy);
		dst += 2;

		s32 b_startx = b_cx - b_dx * b_dispx + b_dmx * b_dispy;
		s32 b_starty = b_cy - b_dy * b_dispx - b_dmy * b_dispy;

		write_memory32(dst, b_startx);
		dst += 4;
		write_memory32(dst, b_starty);
		dst += 4;
	}
}

static void bios_objaffineset(u32 b_src, u32 dst, u32 b_num, u32 b_offset)
{
	u32 b_i;
	for(b_i = 0; b_i < b_num; b_i++) {
		s16 b_rx = read_memory16(b_src);
		b_src+=2;
		s16 b_ry = read_memory16(b_src);
		b_src+=2;
		u16 b_theta = read_memory16(b_src)>>8;
		b_src+=4; // keep structure alignment

		s32 b_a = (s32)b_sinetable[(b_theta+0x40)&255];
		s32 b_b = (s32)b_sinetable[b_theta];

		s16 b_dx =  (s32)((b_rx * b_a)>>14);
		s16 b_dmx = (s32)((b_rx * b_b)>>14);
		s16 b_dy =  (s32)((b_ry * b_b)>>14);
		s16 b_dmy = (s32)((b_ry * b_a)>>14);

		write_memory16(dst, b_dx);
		dst += b_offset;
		write_memory16(dst, -b_dmx);
		dst += b_offset;
		write_memory16(dst, b_dy);
		dst += b_offset;
		write_memory16(dst, b_dmy);
		dst += b_offset;
	}
}

int swi_hle_handle[256][3] = {
	/* Handler address, nb params, value returned */
  { 0, 0, 0, },							// SWI 0:  SoftReset
  { 0, 0, 0, },							// SWI 1:  RegisterRAMReset
  { 0, 0, 0, },							// SWI 2:  Halt
  { 0, 0, 0, },							// SWI 3:  Stop/Sleep
  { 0, 0, 0, },							// SWI 4:  IntrWait
  { 0, 0, 0, },							// SWI 5:  VBlankIntrWait
  { 1, 0, 0, },							// SWI 6:  Div
  { 1, 0, 0, },							// SWI 7:  DivArm
  { (int) bios_sqrt, 1, 1, },			// SWI 8:  Sqrt
  { (int) bios_arctan, 1, 1, },			// SWI 9:  ArcTan
  { (int) bios_arctan2, 2, 1, },		// SWI A:  ArcTan2
  { (int) bios_cpuset, 3, 0, },			// SWI B:  CpuSet
  { (int) bios_cpufastset, 3, 0, },		// SWI C:  CpuFastSet
  { 0, 0, 0, },							// SWI D:  GetBIOSCheckSum
  { (int) bios_bgaffineset, 3, 0, },	// SWI E:  BgAffineSet
  { (int) bios_objaffineset, 4, 0, },	// SWI F:  ObjAffineSet
  { 0, 0, 0, },							// SWI 10: BitUnpack
  { 0, 0, 0, },							// SWI 11: LZ77UnCompWram
  { 0, 0, 0, },							// SWI 12: LZ77UnCompVram
  { 0, 0, 0, },							// SWI 13: HuffUnComp
  { 0, 0, 0, },							// SWI 14: RLUnCompWram
  { 0, 0, 0, },							// SWI 15: RLUnCompVram
  { 0, 0, 0, },							// SWI 16: Diff8bitUnFilterWram
  { 0, 0, 0, },							// SWI 17: Diff8bitUnFilterVram
  { 0, 0, 0, },							// SWI 18: Diff16bitUnFilter
  { 0, 0, 0, },							// SWI 19: SoundBias
  { 0, 0, 0, },							// SWI 1A: SoundDriverInit
  { 0, 0, 0, },							// SWI 1B: SoundDriverMode
  { 0, 0, 0, },							// SWI 1C: SoundDriverMain
  { 0, 0, 0, },							// SWI 1D: SoundDriverVSync
  { 0, 0, 0, },							// SWI 1E: SoundChannelClear
  { 0, 0, 0, },							// SWI 1F: MidiKey2Freq
  { 0, 0, 0, },							// SWI 20: SoundWhatever0
  { 0, 0, 0, },							// SWI 21: SoundWhatever1
  { 0, 0, 0, },							// SWI 22: SoundWhatever2
  { 0, 0, 0, },							// SWI 23: SoundWhatever3
  { 0, 0, 0, },							// SWI 24: SoundWhatever4
  { 0, 0, 0, },							// SWI 25: MultiBoot
  { 0, 0, 0, },							// SWI 26: HardReset
  { 0, 0, 0, },							// SWI 27: CustomHalt
  { 0, 0, 0, },							// SWI 28: SoundDriverVSyncOff
  { 0, 0, 0, },							// SWI 29: SoundDriverVSyncOn
  { 0, 0, 0, },							// SWI 2A: SoundGetJumpList
};

