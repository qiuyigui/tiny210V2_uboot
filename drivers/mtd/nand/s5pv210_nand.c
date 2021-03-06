/*
 * (C) Copyright 2006 DENX Software Engineering
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CONFIG_CMD_NAND)
#include <nand.h>
#include <bedbug/regs.h>


#include <asm/io.h>
#include <asm/errno.h>

#define __REG(a)                  *((volatile unsigned int *)(a))
#define ELFIN_GPIO_BASE			0xE0200000
#define MP01CON_OFFSET 			0x2E0
#define MP01DAT_OFFSET 			0x2E4
#define MP01PUD_OFFSET 			0x2E8
#define MP01DRV_SR_OFFSET 		0x2EC
#define MP01CONPDN_OFFSET 		0x2E0
#define MP01PUDPDN_OFFSET 		0x2E4



#define MP03CON_OFFSET                  0x320
#define MP03DAT_OFFSET                  0x324
#define MP03PUD_OFFSET                  0x328
#define MP03DRV_SR_OFFSET               0x32c
#define MP03CONPDN_OFFSET               0x330
#define MP03PUDPDN_OFFSET               0x334


#define MP01CON_REG			__REG(ELFIN_GPIO_BASE + MP01CON_OFFSET)
#define MP01DAT_REG			__REG(ELFIN_GPIO_BASE + MP01DAT_OFFSET)
#define MP01PUD_REG			__REG(ELFIN_GPIO_BASE + MP01PUD_OFFSET)
#define MP01DRV_REG			__REG(ELFIN_GPIO_BASE + MP01DRV_SR_OFFSET)
#define MP01CONPDN_REG			__REG(ELFIN_GPIO_BASE + MP01CONPDN_OFFSET)
#define MP01PUDPDN_REG			__REG(ELFIN_GPIO_BASE + MP01PUDPDN_OFFSET)



#define MP03CON_REG                     __REG(ELFIN_GPIO_BASE + MP03CON_OFFSET)
#define MP03DAT_REG                     __REG(ELFIN_GPIO_BASE + MP03DAT_OFFSET)
#define MP03PUD_REG                     __REG(ELFIN_GPIO_BASE + MP03PUD_OFFSET)
#define MP03DRV_REG                     __REG(ELFIN_GPIO_BASE + MP03DRV_SR_OFFSET)
#define MP03CONPDN_REG                  __REG(ELFIN_GPIO_BASE + MP03CONPDN_OFFSET)
#define MP03PUDPDN_REG                  __REG(ELFIN_GPIO_BASE + MP03PUDPDN_OFFSET)

#define NFCONF_VAL	(7<<12)|(7<<8)|(7<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0)
#define NFCONT_VAL      (0x1<<23)|(0x1<<22)|(0<<18)|(0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(0<<7)|(0<<6)|(0x2<<1)|(1<<0)
#define MP03CON_VAL	(1<<29)|(1<<25)|(1<<21)|(1<<17)|(1<<13)|(1<<9)|(1<<5)|(1<<1)
/**/
/*
 * Nand flash controller
 */
#define ELFIN_NAND_BASE			0xB0E00000
#define ELFIN_NAND_ECC_BASE		0xB0E20000

#define NFCONF_OFFSET           	0x00
#define NFCONT_OFFSET           	0x04
#define NFCMMD_OFFSET           	0x08
#define NFADDR_OFFSET           	0x0c
#define NFDATA_OFFSET			0x10
#define NFMECCDATA0_OFFSET      	0x14
#define NFMECCDATA1_OFFSET      	0x18
#define NFSECCDATA0_OFFSET      	0x1c
#define NFSBLK_OFFSET           	0x20
#define NFEBLK_OFFSET           	0x24
#define NFSTAT_OFFSET           	0x28
#define NFESTAT0_OFFSET         	0x2c
#define NFESTAT1_OFFSET         	0x30
#define NFMECC0_OFFSET          	0x34
#define NFMECC1_OFFSET          	0x38
#define NFSECC_OFFSET           	0x3c
#define NFMLCBITPT_OFFSET       	0x40
#define NFECCCONF_OFFSET 		0x000 // R/W ECC configuration register 0x0000_0000
#define NFECCCONT_OFFSET 		0x020 // R/W ECC control register 0x0000_0000
#define NFECCSTAT_OFFSET 		0x030 // R ECC status register 0x0000_0000
#define NFECCSECSTAT_OFFSET 		0x040 // R ECC sector status register 0x0000_0000
#define NFECCPRGECC0_OFFSET 		0x090 // R ECC parity code0 register for page program 0x0000_0000
#define NFECCPRGECC1_OFFSET 		0x094 // R ECC parity code1 register for page program 0x0000_0000
#define NFECCPRGECC2_OFFSET 		0x098 // R ECC parity code2 register for page program 0x0000_0000
#define NFECCPRGECC3_OFFSET 		0x09C // R ECC parity code3 register for page program 0x0000_0000
#define NFECCPRGECC4_OFFSET 		0x0A0 // R ECC parity code4 register for page program 0x0000_0000
#define NFECCPRGECC5_OFFSET 		0x0A4 // R ECC parity code5 register for page program 0x0000_0000
#define NFECCPRGECC6_OFFSET 		0x0A8 // R ECC parity code6 register for page program 0x0000_0000
#define NFECCERL0_OFFSET		0x0C0 // R ECC error byte location0 register 0x0000_0000
#define NFECCERL1_OFFSET		0x0C4 // R ECC error byte location1 register 0x0000_0000
#define NFECCERL2_OFFSET		0x0C8 // R ECC error byte location2 register 0x0000_0000
#define NFECCERL3_OFFSET 		0x0CC // R ECC error byte location3 register 0x0000_0000
#define NFECCERL4_OFFSET 		0x0D0 // R ECC error byte location4 register 0x0000_0000
#define NFECCERL5_OFFSET 		0x0D4 // R ECC error byte location5 register 0x0000_0000
#define NFECCERL6_OFFSET 		0x0D8 // R ECC error byte location6 register 0x0000_0000
#define NFECCERL7_OFFSET 		0x0DC // R ECC error byte location7 register 0x0000_0000
#define NFECCERP0_OFFSET 		0x0F0 // R ECC error bit pattern0 register 0x0000_0000
#define NFECCERP1_OFFSET 		0x0F4 // R ECC error bit pattern1 register 0x0000_0000
#define NFECCERP2_OFFSET 		0x0F8 // R ECC error bit pattern2 register 0x0000_0000
#define NFECCERP3_OFFSET 		0x0FC // R ECC error bit pattern3 register 0x0000_0000
#define NFECCCONECC0_OFFSET 		0x110 // R/W ECC parity conversion code0 register 0x0000_0000
#define NFECCCONECC1_OFFSET 		0x114 // R/W ECC parity conversion code1 register 0x0000_0000
#define NFECCCONECC2_OFFSET 		0x118 // R/W ECC parity conversion code2 register 0x0000_0000
#define NFECCCONECC3_OFFSET 		0x11C // R/W ECC parity conversion code3 register 0x0000_0000
#define NFECCCONECC4_OFFSET 		0x120 // R/W ECC parity conversion code4 register 0x0000_0000
#define NFECCCONECC5_OFFSET 		0x124 // R/W ECC parity conversion code5 register 0x0000_0000
#define NFECCCONECC6_OFFSET		0x128 // R/W ECC parity conversion code6 register 0x0000_0000

#define NFCONF				(ELFIN_NAND_BASE+NFCONF_OFFSET)
#define NFCONT				(ELFIN_NAND_BASE+NFCONT_OFFSET)
#define NFCMMD				(ELFIN_NAND_BASE+NFCMMD_OFFSET)
#define NFADDR           		(ELFIN_NAND_BASE+NFADDR_OFFSET)
#define NFDATA          		(ELFIN_NAND_BASE+NFDATA_OFFSET)
#define NFMECCDATA0     		(ELFIN_NAND_BASE+NFMECCDATA0_OFFSET)
#define NFMECCDATA1     		(ELFIN_NAND_BASE+NFMECCDATA1_OFFSET)
#define NFSECCDATA0      		(ELFIN_NAND_BASE+NFSECCDATA0_OFFSET)
#define NFSBLK          		(ELFIN_NAND_BASE+NFSBLK_OFFSET)
#define NFEBLK           		(ELFIN_NAND_BASE+NFEBLK_OFFSET)
#define NFSTAT           		(ELFIN_NAND_BASE+NFSTAT_OFFSET)
#define NFESTAT0         		(ELFIN_NAND_BASE+NFESTAT0_OFFSET)
#define NFESTAT1         		(ELFIN_NAND_BASE+NFESTAT1_OFFSET)
#define NFMECC0          		(ELFIN_NAND_BASE+NFMECC0_OFFSET)
#define NFMECC1          		(ELFIN_NAND_BASE+NFMECC1_OFFSET)
#define NFSECC           		(ELFIN_NAND_BASE+NFSECC_OFFSET)
#define NFMLCBITPT           		(ELFIN_NAND_BASE+NFMLCBITPT_OFFSET)

#define NFECCCONF			(ELFIN_NAND_ECC_BASE+NFECCCONF_OFFSET)
#define NFECCCONT			(ELFIN_NAND_ECC_BASE+NFECCCONT_OFFSET)
#define NFECCSTAT			(ELFIN_NAND_ECC_BASE+NFECCSTAT_OFFSET)
#define NFECCSECSTAT			(ELFIN_NAND_ECC_BASE+NFECCSECSTAT_OFFSET)
#define NFECCPRGECC0			(ELFIN_NAND_ECC_BASE+NFECCPRGECC0_OFFSET)
#define NFECCPRGECC1			(ELFIN_NAND_ECC_BASE+NFECCPRGECC1_OFFSET)
#define NFECCPRGECC2			(ELFIN_NAND_ECC_BASE+NFECCPRGECC2_OFFSET)
#define NFECCPRGECC3			(ELFIN_NAND_ECC_BASE+NFECCPRGECC3_OFFSET)
#define NFECCPRGECC4			(ELFIN_NAND_ECC_BASE+NFECCPRGECC4_OFFSET)
#define NFECCPRGECC5			(ELFIN_NAND_ECC_BASE+NFECCPRGECC5_OFFSET)
#define NFECCPRGECC6			(ELFIN_NAND_ECC_BASE+NFECCPRGECC6_OFFSET)
#define NFECCERL0			(ELFIN_NAND_ECC_BASE+NFECCERL0_OFFSET)
#define NFECCERL1			(ELFIN_NAND_ECC_BASE+NFECCERL1_OFFSET)
#define NFECCERL2			(ELFIN_NAND_ECC_BASE+NFECCERL2_OFFSET)
#define NFECCERL3			(ELFIN_NAND_ECC_BASE+NFECCERL3_OFFSET)
#define NFECCERL4			(ELFIN_NAND_ECC_BASE+NFECCERL4_OFFSET)
#define NFECCERL5			(ELFIN_NAND_ECC_BASE+NFECCERL5_OFFSET)
#define NFECCERL6			(ELFIN_NAND_ECC_BASE+NFECCERL6_OFFSET)
#define NFECCERL7			(ELFIN_NAND_ECC_BASE+NFECCERL7_OFFSET)
#define NFECCERP0			(ELFIN_NAND_ECC_BASE+NFECCERP0_OFFSET)
#define NFECCERP1			(ELFIN_NAND_ECC_BASE+NFECCERP1_OFFSET)
#define NFECCERP2			(ELFIN_NAND_ECC_BASE+NFECCERP2_OFFSET)
#define NFECCERP3			(ELFIN_NAND_ECC_BASE+NFECCERP3_OFFSET)
#define NFECCCONECC0			(ELFIN_NAND_ECC_BASE+NFECCCONECC0_OFFSET)
#define NFECCCONECC1			(ELFIN_NAND_ECC_BASE+NFECCCONECC1_OFFSET)
#define NFECCCONECC2			(ELFIN_NAND_ECC_BASE+NFECCCONECC2_OFFSET)
#define NFECCCONECC3			(ELFIN_NAND_ECC_BASE+NFECCCONECC3_OFFSET)
#define NFECCCONECC4			(ELFIN_NAND_ECC_BASE+NFECCCONECC4_OFFSET)
#define NFECCCONECC5			(ELFIN_NAND_ECC_BASE+NFECCCONECC5_OFFSET)
#define NFECCCONECC6			(ELFIN_NAND_ECC_BASE+NFECCCONECC6_OFFSET)


#define NFCONF_REG			__REG(ELFIN_NAND_BASE+NFCONF_OFFSET)
#define NFCONT_REG			__REG(ELFIN_NAND_BASE+NFCONT_OFFSET)
#define NFCMD_REG			__REG(ELFIN_NAND_BASE+NFCMMD_OFFSET)
#define NFADDR_REG           		__REG(ELFIN_NAND_BASE+NFADDR_OFFSET)
#define NFDATA_REG          		__REG(ELFIN_NAND_BASE+NFDATA_OFFSET)
#define NFDATA8_REG          		__REGb(ELFIN_NAND_BASE+NFDATA_OFFSET)
#define NFMECCDATA0_REG     		__REG(ELFIN_NAND_BASE+NFMECCDATA0_OFFSET)
#define NFMECCDATA1_REG     		__REG(ELFIN_NAND_BASE+NFMECCDATA1_OFFSET)
#define NFSECCDATA0_REG      		__REG(ELFIN_NAND_BASE+NFSECCDATA0_OFFSET)
#define NFSBLK_REG          		__REG(ELFIN_NAND_BASE+NFSBLK_OFFSET)
#define NFEBLK_REG           		__REG(ELFIN_NAND_BASE+NFEBLK_OFFSET)
#define NFSTAT_REG           		__REG(ELFIN_NAND_BASE+NFSTAT_OFFSET)
#define NFESTAT0_REG         		__REG(ELFIN_NAND_BASE+NFESTAT0_OFFSET)
#define NFESTAT1_REG         		__REG(ELFIN_NAND_BASE+NFESTAT1_OFFSET)
#define NFMECC0_REG          		__REG(ELFIN_NAND_BASE+NFMECC0_OFFSET)
#define NFMECC1_REG          		__REG(ELFIN_NAND_BASE+NFMECC1_OFFSET)
#define NFSECC_REG           		__REG(ELFIN_NAND_BASE+NFSECC_OFFSET)
#define NFMLCBITPT_REG         		__REG(ELFIN_NAND_BASE+NFMLCBITPT_OFFSET)

#define NFCONF_ECC_MLC			(1<<24)

#define NFCONF_ECC_1BIT			(0<<23)
#define NFCONF_ECC_4BIT			(2<<23)
#define NFCONF_ECC_8BIT			(1<<23)

#define NFCONT_ECC_ENC			(1<<18)
#define NFCONT_WP			(1<<16)
#define NFCONT_MECCLOCK			(1<<7)
#define NFCONT_SECCLOCK			(1<<6)
#define NFCONT_INITMECC			(1<<5)
#define NFCONT_INITSECC			(1<<4)
#define NFCONT_INITECC			(NFCONT_INITMECC | NFCONT_INITSECC)
#define NFCONT_CS			(1<<1)
#define NFSTAT_ECCENCDONE		(1<<25)
#define NFSTAT_ECCDECDONE		(1<<24)
#define NFSTAT_RnB			(1<<0)
#define NFESTAT0_ECCBUSY		(1<<31)
/**/

/* Nand flash definition values by jsgood */
#define S3C_NAND_TYPE_UNKNOWN	0x0
#define S3C_NAND_TYPE_SLC	0x1
#define S3C_NAND_TYPE_MLC	0x2
#undef	S3C_NAND_DEBUG

/*
 * Hardware specific access to control-lines function
 * Written by jsgood
 */
static void s3c_nand_hwcontrol(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	unsigned int cur;

#if 1
	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_NCE) {
			if (dat != NAND_CMD_NONE) {
				cur = readl(NFCONT);
				/* Forced Enable CS */
				cur &= ~NFCONT_CS;

				writel(cur, NFCONT);
			}
		} else {
			cur = readl(NFCONT);
			/* Forced Enable CS */
			cur |= NFCONT_CS;

			writel(cur, NFCONT);
		}
	}

	if (dat != NAND_CMD_NONE) {
		if (ctrl & NAND_CLE)
			writeb(dat, NFCMMD);
		else if (ctrl & NAND_ALE)
			writeb(dat, NFADDR);
	}
#endif
}

/*
 * Function for checking device ready pin
 * Written by jsgood
 */
static int s3c_nand_device_ready(struct mtd_info *mtdinfo)
{
	while (!(readl(NFSTAT) & NFSTAT_RnB)) {}
	return 1;
}

/*
 * We don't use bad block table
 */
static int s3c_nand_scan_bbt(struct mtd_info *mtdinfo)
{
	return nand_default_bbt(mtdinfo);
}



/*
 * Board-specific NAND initialization. The following members of the
 * argument are board-specific (per include/linux/mtd/nand.h):
 * - IO_ADDR_R?: address to read the 8 I/O lines of the flash device
 * - IO_ADDR_W?: address to write the 8 I/O lines of the flash device
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - enable_hwecc?: function to enable (reset)  hardware ecc generator. Must
 *   only be provided if a hardware ECC is available
 * - eccmode: mode of ecc, see defines
 * - chip_delay: chip dependent delay for transfering data from array to
 *   read regs (tR)
 * - options: various chip options. They can partly be set to inform
 *   nand_scan about special functionality. See the defines for further
 *   explanation
 * Members with a "?" were not set in the merged testing-NAND branch,
 * so they are not set here either.
 */
int board_nand_init(struct nand_chip *nand)
{
	
	unsigned int  cur;

	/*Modified by lk*/
        cur = MP01CON_REG;
	cur = (0x3<<12)|(0x3<<8)|(cur&(~(0xff<<8)));
	MP01CON_REG = cur; 
	
	cur = MP03CON_REG;
	cur = (cur&(~(0xfff<<0)));
	cur = (cur&(~(0xf<<16)));
	cur |= (0x2<<16)|(0x2<<8)|(0x2<<4)|(0x2<<0);
	MP03CON_REG = cur; 
	
	NFCONF_REG |= NFCONF_VAL;
	NFCONT_REG |= NFCONT_VAL;
	NFCONT_REG &= ~((0x1<<16)|(0x1<<6)|(0x1<<7));
	/*Modified by lk*/	

	nand->IO_ADDR_R		= (void __iomem *)(NFDATA);
	nand->IO_ADDR_W		= (void __iomem *)(NFDATA);
	nand->cmd_ctrl		= s3c_nand_hwcontrol;
	nand->dev_ready		= s3c_nand_device_ready;
	nand->scan_bbt		= s3c_nand_scan_bbt;
	nand->options		= 0;
	nand->options		|= NAND_USE_FLASH_BBT;


	nand->ecc.mode = NAND_ECC_SOFT;

	return 0;
}
#endif /* (CONFIG_CMD_NAND) */

