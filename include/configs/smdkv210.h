/*
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * HeungJun Kim <riverful.kim@samsung.com>
 * Inki Dae <inki.dae@samsung.com>
 *
 * Configuation settings for the SAMSUNG SMDKC100 board.
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

#ifndef __CONFIG_H
#define __CONFIG_H
#define DEBUG
/* SPL add by qiuyigui 2014-04-23*/  
#define CONFIG_SPL  
#define CONFIG_SPL_FRAMEWORK
/*#define CONFIG_SYS_ICACHE_OFF*/
#define CONFIG_CMD_NAND  
#define CONFIG_SYS_MAX_NAND_DEVICE 1  
#define CONFIG_SYS_NAND_BASE 0xB0E00000 
#define CONFIG_NAND_S5PV210

#define CONFIG_AUTO_COMPLETE /*开启命令自动补全函数的编译*/
/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC100		1	/* which is in a S5PC100 */
#define CONFIG_SMDKC100		1	/* working with SMDKC100 */

#include <asm/arch/cpu.h>		/* get chip and board defs */

#define CONFIG_ARCH_CPU_INIT

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* input clock of PLL: SMDKC100 has 12MHz input clock */
#define CONFIG_SYS_CLK_FREQ		12000000

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x20000000 /*0x30000000*/

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_EDITING

/*
 * Size of malloc() pool
 * 1MB = 0x100000, 0x100000 = 1024 * 1024
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (1 << 20))
/*
 * select serial console configuration
 */
#define CONFIG_SERIAL0			1	/* use SERIAL 0 on SMDKC100 */
#define CONFIG_SERIAL_MULTI		1

/* PWM */
#define CONFIG_PWM			1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_IMLS
/*#undef CONFIG_CMD_NAND*/

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
/*#define CONFIG_CMD_ONENAND*/  /*edit by qiuyigui 2014-06-14*/
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_MTDPARTS

#define CONFIG_BOOTDELAY	3

#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

#define MTDIDS_DEFAULT		"nand0=K9GAG08U0F-nand"
#define MTDPARTS_DEFAULT	"mtdparts=K9GAG08U0F-nand:256k(bootloader)"\
				",128k@0x40000(params)"\
				",3m@0x60000(kernel)"\
				",16m@0x360000(test)"\
				",-(UBI)"

#define NORMAL_MTDPARTS_DEFAULT MTDPARTS_DEFAULT

/*#define CONFIG_BOOTCOMMAND	"run ubifsboot"*/

#define CONFIG_RAMDISK_BOOT	"root=/dev/ram0 rw rootfstype=ext2" \
				" console=ttySAC0,115200n8" \
				" mem=128M"

#define CONFIG_COMMON_BOOT	"console=ttySAC0,115200n8" \
				" mem=128M " \
				" " MTDPARTS_DEFAULT

#define CONFIG_BOOTARGS	"root=/dev/mtdblock5 ubi.mtd=4" \
			" rootfstype=cramfs " CONFIG_COMMON_BOOT

#define CONFIG_UPDATEB	"updateb=onenand erase 0x0 0x40000;" \
			" onenand write 0x32008000 0x0 0x40000\0"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_EXTRA_ENV_SETTINGS					\
	CONFIG_UPDATEB \
	"updatek=" \
		"onenand erase 0x60000 0x300000;" \
		"onenand write 0x31008000 0x60000 0x300000\0" \
	"updateu=" \
		"onenand erase block 147-4095;" \
		"onenand write 0x32000000 0x1260000 0x8C0000\0" \
	"bootk=" \
		"onenand read 0x30007FC0 0x60000 0x300000;" \
		"bootm 0x30007FC0\0" \
	"flashboot=" \
		"set bootargs root=/dev/mtdblock${bootblock} " \
		"rootfstype=${rootfstype} " \
		"ubi.mtd=${ubiblock} ${opts} " CONFIG_COMMON_BOOT ";" \
		"run bootk\0" \
	"ubifsboot=" \
		"set bootargs root=ubi0!rootfs rootfstype=ubifs " \
		" ubi.mtd=${ubiblock} ${opts} " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"boottrace=setenv opts initcall_debug; run bootcmd\0" \
	"android=" \
		"set bootargs root=ubi0!ramdisk ubi.mtd=${ubiblock} " \
		"rootfstype=ubifs init=/init.sh " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"nfsboot=" \
		"set bootargs root=/dev/nfs ubi.mtd=${ubiblock} " \
		"nfsroot=${nfsroot},nolock " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:" \
		"${netmask}:nowplus:usb0:off " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"ramboot=" \
		"set bootargs " CONFIG_RAMDISK_BOOT \
		" initrd=0x33000000,8M ramdisk=8192\0" \
	"rootfstype=cramfs\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"meminfo=mem=128M\0" \
	"nfsroot=/nfsroot/arm\0" \
	"bootblock=5\0" \
	"ubiblock=4\0" \
	"ubi=enabled"

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
/*#define CONFIG_SYS_HUSH_PARSER		 use "hush" command parser	*/
#define CONFIG_SYS_PROMPT		"SMDKV210 # "
#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5e00000)
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE

#define CONFIG_SYS_HZ			1000

/* SMDKC100 has 1 banks of DRAM, we use only one in U-Boot */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	(512 << 20)	/* 0x8000000, 128 MB Bank #1 */

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH		1

#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 KiB */
#define CONFIG_IDENT_STRING		" for SMDKC100"

#if !defined(CONFIG_NAND_SPL) && (CONFIG_SYS_TEXT_BASE >= 0xc0000000)
#define CONFIG_ENABLE_MMU
#endif

#ifdef CONFIG_ENABLE_MMU
#define CONFIG_SYS_MAPPED_RAM_BASE	0xc0000000
#else
#define CONFIG_SYS_MAPPED_RAM_BASE	CONFIG_SYS_SDRAM_BASE
#endif

/*-----------------------------------------------------------------------
 * Boot configuration
 */
#define CONFIG_CMD_SAVEENV              1
#define CONFIG_ENV_IS_IN_NAND           1
#define CONFIG_ENV_IS_IN_ONENAND	0               /*edit by qiuyigui 2014-06-14*/
#define CONFIG_ENV_SIZE			(1 << 20)	/* 128KiB, 0x20000 */
#define CONFIG_ENV_ADDR			(1 << 20)	/* 256KiB, 0x40000 */
#define CONFIG_ENV_OFFSET		(1 << 20)	/* 256KiB, 0x40000 */

/*#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SAMSUNG_ONENAND		1
#define CONFIG_SYS_ONENAND_BASE		0xE7100000*/

#define CONFIG_DOS_PARTITION		1

#define CONFIG_SYS_INIT_SP_ADDR	(0x30000000) //(CONFIG_SYS_LOAD_ADDR - 0x1000000)

/*
 * Ethernet Contoller driver
 */
#define CONFIG_CMD_NET
#ifdef  CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define DM9000_16BIT_DATA
#define CONFIG_DRIVER_DM9000       1
#define CONFIG_NET_MULTI               1
#define CONFIG_NET_RETRY_COUNT 1
#define CONFIG_DM9000_NO_SROM 1
#ifdef  CONFIG_DRIVER_DM9000  
#define CONFIG_DM9000_BASE              (0x88001000)
#define DM9000_IO                       (CONFIG_DM9000_BASE)
#if defined(DM9000_16BIT_DATA)
#define DM9000_DATA                     (CONFIG_DM9000_BASE+0x300C)
#else
#define DM9000_DATA                     (CONFIG_DM9000_BASE+1)
#endif
#endif
#endif /* CONFIG_CMD_NET */

#endif	/* __CONFIG_H */
