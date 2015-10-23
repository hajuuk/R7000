#ifndef _BBCONFIGOPTS_H
#define _BBCONFIGOPTS_H
/*
 * busybox configuration settings.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 *
 * This file is generated automatically by scripts/mkconfigs.
 * Do not edit.
 *
 */
static const char *const bbconfig_config =
"CONFIG_HAVE_DOT_CONFIG=y\n"
"# CONFIG_NITPICK is not set\n"
"# CONFIG_DESKTOP is not set\n"
"# CONFIG_FEATURE_BUFFERS_USE_MALLOC is not set\n"
"# CONFIG_FEATURE_BUFFERS_GO_ON_STACK is not set\n"
"# CONFIG_FEATURE_BUFFERS_GO_IN_BSS is not set\n"
"CONFIG_SHOW_USAGE=y\n"
"CONFIG_FEATURE_VERBOSE_USAGE=y\n"
"CONFIG_FEATURE_COMPRESS_USAGE=y\n"
"# CONFIG_FEATURE_INSTALLER is not set\n"
"# CONFIG_LOCALE_SUPPORT is not set\n"
"CONFIG_GETOPT_LONG=y\n"
"CONFIG_FEATURE_DEVPTS=y\n"
"# CONFIG_FEATURE_CLEAN_UP is not set\n"
"CONFIG_FEATURE_PIDFILE=y\n"
"CONFIG_FEATURE_SUID=y\n"
"CONFIG_FEATURE_SUID_CONFIG=y\n"
"CONFIG_FEATURE_SUID_CONFIG_QUIET=y\n"
"# CONFIG_SELINUX is not set\n"
"# CONFIG_FEATURE_PREFER_APPLETS is not set\n"
"CONFIG_BUSYBOX_EXEC_PATH=\"/proc/self/exe\"\n"
"CONFIG_FEATURE_SYSLOG=y\n"
"CONFIG_FEATURE_HAVE_RPC=y\n"
"# CONFIG_STATIC is not set\n"
"# CONFIG_BUILD_LIBBUSYBOX is not set\n"
"# CONFIG_FEATURE_FULL_LIBBUSYBOX is not set\n"
"# CONFIG_FEATURE_SHARED_BUSYBOX is not set\n"
"CONFIG_LFS=y\n"
"# CONFIG_BUILD_AT_ONCE is not set\n"
"# CONFIG_DEBUG is not set\n"
"# CONFIG_WERROR is not set\n"
"CONFIG_NO_DEBUG_LIB=y\n"
"# CONFIG_DMALLOC is not set\n"
"# CONFIG_EFENCE is not set\n"
"# CONFIG_INCLUDE_SUSv2 is not set\n"
"# CONFIG_INSTALL_NO_USR is not set\n"
"CONFIG_INSTALL_APPLET_SYMLINKS=y\n"
"# CONFIG_INSTALL_APPLET_HARDLINKS is not set\n"
"# CONFIG_INSTALL_APPLET_DONT is not set\n"
"CONFIG_PREFIX=\"./_install\"\n"
"CONFIG_PASSWORD_MINLEN=6\n"
"CONFIG_MD5_SIZE_VS_SPEED=2\n"
"CONFIG_FEATURE_FAST_TOP=y\n"
"# CONFIG_FEATURE_ETC_NETWORKS is not set\n"
"CONFIG_FEATURE_EDITING=y\n"
"CONFIG_FEATURE_EDITING_MAX_LEN=1024\n"
"CONFIG_FEATURE_EDITING_FANCY_KEYS=y\n"
"# CONFIG_FEATURE_EDITING_VI is not set\n"
"CONFIG_FEATURE_EDITING_HISTORY=15\n"
"# CONFIG_FEATURE_EDITING_SAVEHISTORY is not set\n"
"CONFIG_FEATURE_TAB_COMPLETION=y\n"
"# CONFIG_FEATURE_USERNAME_COMPLETION is not set\n"
"# CONFIG_FEATURE_EDITING_FANCY_PROMPT is not set\n"
"# CONFIG_MONOTONIC_SYSCALL is not set\n"
"# CONFIG_IOCTL_HEX2STR_ERROR is not set\n"
"# CONFIG_AR is not set\n"
"# CONFIG_FEATURE_AR_LONG_FILENAMES is not set\n"
"# CONFIG_BUNZIP2 is not set\n"
"# CONFIG_CPIO is not set\n"
"# CONFIG_DPKG is not set\n"
"# CONFIG_DPKG_DEB is not set\n"
"# CONFIG_FEATURE_DPKG_DEB_EXTRACT_ONLY is not set\n"
"CONFIG_GUNZIP=y\n"
"CONFIG_FEATURE_GUNZIP_UNCOMPRESS=y\n"
"CONFIG_GZIP=y\n"
"# CONFIG_RPM2CPIO is not set\n"
"# CONFIG_RPM is not set\n"
"# CONFIG_FEATURE_RPM_BZ2 is not set\n"
"CONFIG_TAR=y\n"
"CONFIG_FEATURE_TAR_CREATE=y\n"
"# CONFIG_FEATURE_TAR_BZIP2 is not set\n"
"# CONFIG_FEATURE_TAR_LZMA is not set\n"
"# CONFIG_FEATURE_TAR_FROM is not set\n"
"CONFIG_FEATURE_TAR_GZIP=y\n"
"# CONFIG_FEATURE_TAR_COMPRESS is not set\n"
"# CONFIG_FEATURE_TAR_OLDGNU_COMPATIBILITY is not set\n"
"# CONFIG_FEATURE_TAR_OLDSUN_COMPATIBILITY is not set\n"
"CONFIG_FEATURE_TAR_GNU_EXTENSIONS=y\n"
"CONFIG_FEATURE_TAR_LONG_OPTIONS=y\n"
"# CONFIG_UNCOMPRESS is not set\n"
"# CONFIG_UNLZMA is not set\n"
"# CONFIG_FEATURE_LZMA_FAST is not set\n"
"# CONFIG_UNZIP is not set\n"
"# CONFIG_FEATURE_UNARCHIVE_TAPE is not set\n"
"# CONFIG_FEATURE_DEB_TAR_GZ is not set\n"
"# CONFIG_FEATURE_DEB_TAR_BZ2 is not set\n"
"# CONFIG_FEATURE_DEB_TAR_LZMA is not set\n"
"CONFIG_BASENAME=y\n"
"# CONFIG_CAL is not set\n"
"CONFIG_CAT=y\n"
"# CONFIG_CATV is not set\n"
"# CONFIG_CHGRP is not set\n"
"CONFIG_CHMOD=y\n"
"# CONFIG_CHOWN is not set\n"
"# CONFIG_CHROOT is not set\n"
"# CONFIG_CKSUM is not set\n"
"# CONFIG_COMM is not set\n"
"CONFIG_CP=y\n"
"CONFIG_CUT=y\n"
"CONFIG_DATE=y\n"
"# CONFIG_FEATURE_DATE_ISOFMT is not set\n"
"# CONFIG_DD is not set\n"
"# CONFIG_FEATURE_DD_SIGNAL_HANDLING is not set\n"
"# CONFIG_FEATURE_DD_IBS_OBS is not set\n"
"CONFIG_DF=y\n"
"CONFIG_DIRNAME=y\n"
"# CONFIG_DOS2UNIX is not set\n"
"# CONFIG_UNIX2DOS is not set\n"
"CONFIG_DU=y\n"
"CONFIG_FEATURE_DU_DEFAULT_BLOCKSIZE_1K=y\n"
"CONFIG_ECHO=y\n"
"CONFIG_FEATURE_FANCY_ECHO=y\n"
"CONFIG_ENV=y\n"
"CONFIG_FEATURE_ENV_LONG_OPTIONS=y\n"
"# CONFIG_EXPAND is not set\n"
"# CONFIG_FEATURE_EXPAND_LONG_OPTIONS is not set\n"
"CONFIG_EXPR=y\n"
"# CONFIG_EXPR_MATH_SUPPORT_64 is not set\n"
"CONFIG_FALSE=y\n"
"# CONFIG_FOLD is not set\n"
"CONFIG_HEAD=y\n"
"CONFIG_FEATURE_FANCY_HEAD=y\n"
"CONFIG_HOSTID=y\n"
"CONFIG_ID=y\n"
"# CONFIG_INSTALL is not set\n"
"# CONFIG_FEATURE_INSTALL_LONG_OPTIONS is not set\n"
"# CONFIG_LENGTH is not set\n"
"CONFIG_LN=y\n"
"# CONFIG_LOGNAME is not set\n"
"CONFIG_LS=y\n"
"CONFIG_FEATURE_LS_FILETYPES=y\n"
"CONFIG_FEATURE_LS_FOLLOWLINKS=y\n"
"CONFIG_FEATURE_LS_RECURSIVE=y\n"
"CONFIG_FEATURE_LS_SORTFILES=y\n"
"CONFIG_FEATURE_LS_TIMESTAMPS=y\n"
"CONFIG_FEATURE_LS_USERNAME=y\n"
"CONFIG_FEATURE_LS_COLOR=y\n"
"CONFIG_FEATURE_LS_COLOR_IS_DEFAULT=y\n"
"CONFIG_MD5SUM=y\n"
"CONFIG_MKDIR=y\n"
"# CONFIG_FEATURE_MKDIR_LONG_OPTIONS is not set\n"
"CONFIG_MKFIFO=y\n"
"CONFIG_MKNOD=y\n"
"CONFIG_MV=y\n"
"# CONFIG_FEATURE_MV_LONG_OPTIONS is not set\n"
"CONFIG_NICE=y\n"
"# CONFIG_NOHUP is not set\n"
"# CONFIG_OD is not set\n"
"CONFIG_PRINTENV=y\n"
"CONFIG_PRINTF=y\n"
"CONFIG_PWD=y\n"
"# CONFIG_READLINK is not set\n"
"# CONFIG_FEATURE_READLINK_FOLLOW is not set\n"
"# CONFIG_REALPATH is not set\n"
"CONFIG_RM=y\n"
"CONFIG_RMDIR=y\n"
"# CONFIG_SEQ is not set\n"
"# CONFIG_SHA1SUM is not set\n"
"CONFIG_SLEEP=y\n"
"CONFIG_FEATURE_FANCY_SLEEP=y\n"
"# CONFIG_SORT is not set\n"
"# CONFIG_FEATURE_SORT_BIG is not set\n"
"# CONFIG_SPLIT is not set\n"
"# CONFIG_FEATURE_SPLIT_FANCY is not set\n"
"# CONFIG_STAT is not set\n"
"# CONFIG_FEATURE_STAT_FORMAT is not set\n"
"# CONFIG_STTY is not set\n"
"# CONFIG_SUM is not set\n"
"# CONFIG_SYNC is not set\n"
"CONFIG_TAIL=y\n"
"CONFIG_FEATURE_FANCY_TAIL=y\n"
"# CONFIG_TEE is not set\n"
"# CONFIG_FEATURE_TEE_USE_BLOCK_IO is not set\n"
"CONFIG_TEST=y\n"
"# CONFIG_FEATURE_TEST_64 is not set\n"
"CONFIG_TOUCH=y\n"
"CONFIG_TR=y\n"
"CONFIG_FEATURE_TR_CLASSES=y\n"
"CONFIG_FEATURE_TR_EQUIV=y\n"
"CONFIG_TRUE=y\n"
"# CONFIG_TTY is not set\n"
"CONFIG_UNAME=y\n"
"# CONFIG_UNEXPAND is not set\n"
"# CONFIG_FEATURE_UNEXPAND_LONG_OPTIONS is not set\n"
"# CONFIG_UNIQ is not set\n"
"CONFIG_USLEEP=y\n"
"# CONFIG_UUDECODE is not set\n"
"# CONFIG_UUENCODE is not set\n"
"CONFIG_WC=y\n"
"CONFIG_FEATURE_WC_LARGE=y\n"
"# CONFIG_WHO is not set\n"
"# CONFIG_WHOAMI is not set\n"
"CONFIG_YES=y\n"
"CONFIG_FEATURE_PRESERVE_HARDLINKS=y\n"
"CONFIG_FEATURE_AUTOWIDTH=y\n"
"CONFIG_FEATURE_HUMAN_READABLE=y\n"
"# CONFIG_FEATURE_MD5_SHA1_SUM_CHECK is not set\n"
"# CONFIG_CHVT is not set\n"
"CONFIG_CLEAR=y\n"
"# CONFIG_DEALLOCVT is not set\n"
"# CONFIG_DUMPKMAP is not set\n"
"# CONFIG_LOADFONT is not set\n"
"# CONFIG_LOADKMAP is not set\n"
"# CONFIG_OPENVT is not set\n"
"CONFIG_RESET=y\n"
"# CONFIG_RESIZE is not set\n"
"# CONFIG_FEATURE_RESIZE_PRINT is not set\n"
"# CONFIG_SETCONSOLE is not set\n"
"# CONFIG_FEATURE_SETCONSOLE_LONG_OPTIONS is not set\n"
"# CONFIG_SETKEYCODES is not set\n"
"# CONFIG_SETLOGCONS is not set\n"
"# CONFIG_MKTEMP is not set\n"
"# CONFIG_PIPE_PROGRESS is not set\n"
"# CONFIG_RUN_PARTS is not set\n"
"# CONFIG_FEATURE_RUN_PARTS_LONG_OPTIONS is not set\n"
"# CONFIG_FEATURE_RUN_PARTS_FANCY is not set\n"
"# CONFIG_START_STOP_DAEMON is not set\n"
"# CONFIG_FEATURE_START_STOP_DAEMON_FANCY is not set\n"
"# CONFIG_FEATURE_START_STOP_DAEMON_LONG_OPTIONS is not set\n"
"# CONFIG_WHICH is not set\n"
"CONFIG_AWK=y\n"
"CONFIG_FEATURE_AWK_MATH=y\n"
"# CONFIG_CMP is not set\n"
"# CONFIG_DIFF is not set\n"
"# CONFIG_FEATURE_DIFF_BINARY is not set\n"
"# CONFIG_FEATURE_DIFF_DIR is not set\n"
"# CONFIG_FEATURE_DIFF_MINIMAL is not set\n"
"# CONFIG_ED is not set\n"
"# CONFIG_PATCH is not set\n"
"CONFIG_SED=y\n"
"CONFIG_VI=y\n"
"CONFIG_FEATURE_VI_MAX_LEN=1024\n"
"CONFIG_FEATURE_VI_COLON=y\n"
"CONFIG_FEATURE_VI_YANKMARK=y\n"
"CONFIG_FEATURE_VI_SEARCH=y\n"
"CONFIG_FEATURE_VI_USE_SIGNALS=y\n"
"CONFIG_FEATURE_VI_DOT_CMD=y\n"
"CONFIG_FEATURE_VI_READONLY=y\n"
"CONFIG_FEATURE_VI_SETOPTS=y\n"
"CONFIG_FEATURE_VI_SET=y\n"
"CONFIG_FEATURE_VI_WIN_RESIZE=y\n"
"CONFIG_FEATURE_VI_OPTIMIZE_CURSOR=y\n"
"# CONFIG_FEATURE_ALLOW_EXEC is not set\n"
"CONFIG_FIND=y\n"
"CONFIG_FEATURE_FIND_PRINT0=y\n"
"CONFIG_FEATURE_FIND_MTIME=y\n"
"CONFIG_FEATURE_FIND_MMIN=y\n"
"CONFIG_FEATURE_FIND_PERM=y\n"
"CONFIG_FEATURE_FIND_TYPE=y\n"
"CONFIG_FEATURE_FIND_XDEV=y\n"
"CONFIG_FEATURE_FIND_MAXDEPTH=y\n"
"CONFIG_FEATURE_FIND_NEWER=y\n"
"CONFIG_FEATURE_FIND_INUM=y\n"
"CONFIG_FEATURE_FIND_EXEC=y\n"
"CONFIG_FEATURE_FIND_USER=y\n"
"CONFIG_FEATURE_FIND_GROUP=y\n"
"CONFIG_FEATURE_FIND_NOT=y\n"
"CONFIG_FEATURE_FIND_DEPTH=y\n"
"CONFIG_FEATURE_FIND_PAREN=y\n"
"CONFIG_FEATURE_FIND_SIZE=y\n"
"CONFIG_FEATURE_FIND_PRUNE=y\n"
"CONFIG_FEATURE_FIND_DELETE=y\n"
"CONFIG_FEATURE_FIND_PATH=y\n"
"CONFIG_FEATURE_FIND_REGEX=y\n"
"# CONFIG_FEATURE_FIND_CONTEXT is not set\n"
"CONFIG_GREP=y\n"
"CONFIG_FEATURE_GREP_EGREP_ALIAS=y\n"
"CONFIG_FEATURE_GREP_FGREP_ALIAS=y\n"
"CONFIG_FEATURE_GREP_CONTEXT=y\n"
"CONFIG_XARGS=y\n"
"# CONFIG_FEATURE_XARGS_SUPPORT_CONFIRMATION is not set\n"
"# CONFIG_FEATURE_XARGS_SUPPORT_QUOTES is not set\n"
"# CONFIG_FEATURE_XARGS_SUPPORT_TERMOPT is not set\n"
"# CONFIG_FEATURE_XARGS_SUPPORT_ZERO_TERM is not set\n"
"# CONFIG_INIT is not set\n"
"# CONFIG_DEBUG_INIT is not set\n"
"# CONFIG_FEATURE_USE_INITTAB is not set\n"
"# CONFIG_FEATURE_INIT_SCTTY is not set\n"
"# CONFIG_FEATURE_INIT_SYSLOG is not set\n"
"# CONFIG_FEATURE_EXTRA_QUIET is not set\n"
"# CONFIG_FEATURE_INIT_COREDUMPS is not set\n"
"# CONFIG_FEATURE_INITRD is not set\n"
"CONFIG_HALT=y\n"
"# CONFIG_MESG is not set\n"
"CONFIG_FEATURE_SHADOWPASSWDS=y\n"
"# CONFIG_USE_BB_SHADOW is not set\n"
"# CONFIG_USE_BB_PWD_GRP is not set\n"
"CONFIG_ADDGROUP=y\n"
"# CONFIG_FEATURE_ADDUSER_TO_GROUP is not set\n"
"CONFIG_DELGROUP=y\n"
"# CONFIG_FEATURE_DEL_USER_FROM_GROUP is not set\n"
"CONFIG_ADDUSER=y\n"
"CONFIG_DELUSER=y\n"
"# CONFIG_GETTY is not set\n"
"# CONFIG_FEATURE_UTMP is not set\n"
"# CONFIG_FEATURE_WTMP is not set\n"
"# CONFIG_LOGIN is not set\n"
"# CONFIG_PAM is not set\n"
"# CONFIG_LOGIN_SCRIPTS is not set\n"
"# CONFIG_FEATURE_NOLOGIN is not set\n"
"# CONFIG_FEATURE_SECURETTY is not set\n"
"CONFIG_PASSWD=y\n"
"CONFIG_FEATURE_PASSWD_WEAK_CHECK=y\n"
"# CONFIG_CRYPTPW is not set\n"
"# CONFIG_CHPASSWD is not set\n"
"CONFIG_SU=y\n"
"# CONFIG_FEATURE_SU_SYSLOG is not set\n"
"# CONFIG_FEATURE_SU_CHECKS_SHELLS is not set\n"
"# CONFIG_SULOGIN is not set\n"
"# CONFIG_VLOCK is not set\n"
"# CONFIG_CHATTR is not set\n"
"# CONFIG_FSCK is not set\n"
"# CONFIG_LSATTR is not set\n"
"CONFIG_INSMOD=y\n"
"# CONFIG_FEATURE_INSMOD_VERSION_CHECKING is not set\n"
"CONFIG_FEATURE_INSMOD_KSYMOOPS_SYMBOLS=y\n"
"CONFIG_FEATURE_INSMOD_LOADINKMEM=y\n"
"CONFIG_FEATURE_INSMOD_LOAD_MAP=y\n"
"CONFIG_FEATURE_INSMOD_LOAD_MAP_FULL=y\n"
"CONFIG_RMMOD=y\n"
"CONFIG_LSMOD=y\n"
"CONFIG_FEATURE_LSMOD_PRETTY_2_6_OUTPUT=y\n"
"# CONFIG_MODPROBE is not set\n"
"# CONFIG_FEATURE_MODPROBE_MULTIPLE_OPTIONS is not set\n"
"# CONFIG_FEATURE_MODPROBE_FANCY_ALIAS is not set\n"
"CONFIG_FEATURE_CHECK_TAINTED_MODULE=y\n"
"CONFIG_FEATURE_2_4_MODULES=y\n"
"CONFIG_FEATURE_2_6_MODULES=y\n"
"# CONFIG_FEATURE_QUERY_MODULE_INTERFACE is not set\n"
"CONFIG_DMESG=y\n"
"CONFIG_FEATURE_DMESG_PRETTY=y\n"
"# CONFIG_FBSET is not set\n"
"# CONFIG_FEATURE_FBSET_FANCY is not set\n"
"# CONFIG_FEATURE_FBSET_READMODE is not set\n"
"# CONFIG_FDFLUSH is not set\n"
"# CONFIG_FDFORMAT is not set\n"
"# CONFIG_FDISK is not set\n"
"CONFIG_FDISK_SUPPORT_LARGE_DISKS=y\n"
"# CONFIG_FEATURE_FDISK_WRITABLE is not set\n"
"# CONFIG_FEATURE_AIX_LABEL is not set\n"
"# CONFIG_FEATURE_SGI_LABEL is not set\n"
"# CONFIG_FEATURE_SUN_LABEL is not set\n"
"# CONFIG_FEATURE_OSF_LABEL is not set\n"
"# CONFIG_FEATURE_FDISK_ADVANCED is not set\n"
"# CONFIG_FREERAMDISK is not set\n"
"# CONFIG_FSCK_MINIX is not set\n"
"# CONFIG_MKFS_MINIX is not set\n"
"# CONFIG_FEATURE_MINIX2 is not set\n"
"# CONFIG_GETOPT is not set\n"
"# CONFIG_HEXDUMP is not set\n"
"# CONFIG_HWCLOCK is not set\n"
"# CONFIG_FEATURE_HWCLOCK_LONG_OPTIONS is not set\n"
"# CONFIG_FEATURE_HWCLOCK_ADJTIME_FHS is not set\n"
"# CONFIG_IPCRM is not set\n"
"# CONFIG_IPCS is not set\n"
"# CONFIG_LOSETUP is not set\n"
"# CONFIG_MDEV is not set\n"
"# CONFIG_FEATURE_MDEV_CONF is not set\n"
"# CONFIG_FEATURE_MDEV_EXEC is not set\n"
"# CONFIG_FEATURE_MDEV_LOAD_FIRMWARE is not set\n"
"# CONFIG_MKSWAP is not set\n"
"# CONFIG_FEATURE_MKSWAP_V0 is not set\n"
"CONFIG_MORE=y\n"
"CONFIG_FEATURE_USE_TERMIOS=y\n"
"CONFIG_MOUNT=y\n"
"CONFIG_FEATURE_MOUNT_NFS=y\n"
"CONFIG_FEATURE_MOUNT_CIFS=y\n"
"CONFIG_FEATURE_MOUNT_FLAGS=y\n"
"CONFIG_FEATURE_MOUNT_FSTAB=y\n"
"CONFIG_PIVOT_ROOT=y\n"
"CONFIG_RDATE=y\n"
"# CONFIG_READPROFILE is not set\n"
"# CONFIG_SETARCH is not set\n"
"# CONFIG_SWAPONOFF is not set\n"
"# CONFIG_SWITCH_ROOT is not set\n"
"CONFIG_UMOUNT=y\n"
"CONFIG_FEATURE_UMOUNT_ALL=y\n"
"CONFIG_FEATURE_MOUNT_LOOP=y\n"
"# CONFIG_FEATURE_MTAB_SUPPORT is not set\n"
"# CONFIG_ADJTIMEX is not set\n"
"# CONFIG_BBCONFIG is not set\n"
"# CONFIG_CHRT is not set\n"
"CONFIG_CROND=y\n"
"CONFIG_DEBUG_CROND_OPTION=y\n"
"# CONFIG_FEATURE_CROND_CALL_SENDMAIL is not set\n"
"CONFIG_CRONTAB=y\n"
"# CONFIG_DC is not set\n"
"# CONFIG_DEVFSD is not set\n"
"# CONFIG_DEVFSD_MODLOAD is not set\n"
"# CONFIG_DEVFSD_FG_NP is not set\n"
"# CONFIG_DEVFSD_VERBOSE is not set\n"
"# CONFIG_FEATURE_DEVFS is not set\n"
"# CONFIG_EJECT is not set\n"
"# CONFIG_LAST is not set\n"
"CONFIG_LESS=y\n"
"CONFIG_FEATURE_LESS_MAXLINES=9999999\n"
"# CONFIG_FEATURE_LESS_BRACKETS is not set\n"
"# CONFIG_FEATURE_LESS_FLAGS is not set\n"
"# CONFIG_FEATURE_LESS_FLAGCS is not set\n"
"# CONFIG_FEATURE_LESS_MARKS is not set\n"
"# CONFIG_FEATURE_LESS_REGEXP is not set\n"
"# CONFIG_HDPARM is not set\n"
"# CONFIG_FEATURE_HDPARM_GET_IDENTITY is not set\n"
"# CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF is not set\n"
"# CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF is not set\n"
"# CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET is not set\n"
"# CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF is not set\n"
"# CONFIG_FEATURE_HDPARM_HDIO_GETSET_DMA is not set\n"
"# CONFIG_MAKEDEVS is not set\n"
"# CONFIG_FEATURE_MAKEDEVS_LEAF is not set\n"
"# CONFIG_FEATURE_MAKEDEVS_TABLE is not set\n"
"# CONFIG_MOUNTPOINT is not set\n"
"# CONFIG_MT is not set\n"
"# CONFIG_RAIDAUTORUN is not set\n"
"# CONFIG_READAHEAD is not set\n"
"# CONFIG_RUNLEVEL is not set\n"
"# CONFIG_RX is not set\n"
"# CONFIG_STRINGS is not set\n"
"# CONFIG_SETSID is not set\n"
"# CONFIG_TASKSET is not set\n"
"# CONFIG_FEATURE_TASKSET_FANCY is not set\n"
"# CONFIG_TIME is not set\n"
"# CONFIG_TTYSIZE is not set\n"
"# CONFIG_WATCHDOG is not set\n"
"CONFIG_FEATURE_IPV6=y\n"
"# CONFIG_VERBOSE_RESOLUTION_ERRORS is not set\n"
"CONFIG_ARP=y\n"
"# CONFIG_ARPING is not set\n"
"# CONFIG_DNSD is not set\n"
"# CONFIG_ETHER_WAKE is not set\n"
"# CONFIG_FAKEIDENTD is not set\n"
"# CONFIG_FTPGET is not set\n"
"# CONFIG_FTPPUT is not set\n"
"# CONFIG_FEATURE_FTPGETPUT_LONG_OPTIONS is not set\n"
"CONFIG_HOSTNAME=y\n"
"# CONFIG_HTTPD is not set\n"
"# CONFIG_FEATURE_HTTPD_USE_SENDFILE is not set\n"
"# CONFIG_FEATURE_HTTPD_RELOAD_CONFIG_SIGHUP is not set\n"
"# CONFIG_FEATURE_HTTPD_SETUID is not set\n"
"# CONFIG_FEATURE_HTTPD_BASIC_AUTH is not set\n"
"# CONFIG_FEATURE_HTTPD_AUTH_MD5 is not set\n"
"# CONFIG_FEATURE_HTTPD_CONFIG_WITH_MIME_TYPES is not set\n"
"# CONFIG_FEATURE_HTTPD_CGI is not set\n"
"# CONFIG_FEATURE_HTTPD_CONFIG_WITH_SCRIPT_INTERPR is not set\n"
"# CONFIG_FEATURE_HTTPD_SET_REMOTE_PORT_TO_ENV is not set\n"
"# CONFIG_FEATURE_HTTPD_ENCODE_URL_STR is not set\n"
"# CONFIG_FEATURE_HTTPD_ERROR_PAGES is not set\n"
"CONFIG_IFCONFIG=y\n"
"CONFIG_FEATURE_IFCONFIG_STATUS=y\n"
"# CONFIG_FEATURE_IFCONFIG_SLIP is not set\n"
"CONFIG_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ=y\n"
"CONFIG_FEATURE_IFCONFIG_HW=y\n"
"CONFIG_FEATURE_IFCONFIG_BROADCAST_PLUS=y\n"
"# CONFIG_IFUPDOWN is not set\n"
"CONFIG_IFUPDOWN_IFSTATE_PATH=\"/var/run/ifstate\"\n"
"# CONFIG_FEATURE_IFUPDOWN_IP is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_IP_BUILTIN is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_IFCONFIG_BUILTIN is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_IPV4 is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_IPV6 is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_MAPPING is not set\n"
"# CONFIG_FEATURE_IFUPDOWN_EXTERNAL_DHCP is not set\n"
"# CONFIG_INETD is not set\n"
"# CONFIG_FEATURE_INETD_SUPPORT_BUILTIN_ECHO is not set\n"
"# CONFIG_FEATURE_INETD_SUPPORT_BUILTIN_DISCARD is not set\n"
"# CONFIG_FEATURE_INETD_SUPPORT_BUILTIN_TIME is not set\n"
"# CONFIG_FEATURE_INETD_SUPPORT_BUILTIN_DAYTIME is not set\n"
"# CONFIG_FEATURE_INETD_SUPPORT_BUILTIN_CHARGEN is not set\n"
"# CONFIG_FEATURE_INETD_RPC is not set\n"
"CONFIG_IP=y\n"
"CONFIG_FEATURE_IP_ADDRESS=y\n"
"CONFIG_FEATURE_IP_LINK=y\n"
"CONFIG_FEATURE_IP_ROUTE=y\n"
"CONFIG_FEATURE_IP_TUNNEL=y\n"
"CONFIG_FEATURE_IP_RULE=y\n"
"CONFIG_FEATURE_IP_SHORT_FORMS=y\n"
"CONFIG_IPADDR=y\n"
"CONFIG_IPLINK=y\n"
"CONFIG_IPROUTE=y\n"
"CONFIG_IPTUNNEL=y\n"
"CONFIG_IPRULE=y\n"
"# CONFIG_IPCALC is not set\n"
"# CONFIG_FEATURE_IPCALC_FANCY is not set\n"
"# CONFIG_FEATURE_IPCALC_LONG_OPTIONS is not set\n"
"# CONFIG_NAMEIF is not set\n"
"# CONFIG_NC is not set\n"
"# CONFIG_NC_SERVER is not set\n"
"# CONFIG_NC_EXTRA is not set\n"
"CONFIG_NETSTAT=y\n"
"CONFIG_FEATURE_NETSTAT_WIDE=y\n"
"CONFIG_NSLOOKUP=y\n"
"CONFIG_PING=y\n"
"CONFIG_PING6=y\n"
"# CONFIG_PSCAN is not set\n"
"CONFIG_FEATURE_FANCY_PING=y\n"
"CONFIG_ROUTE=y\n"
"# CONFIG_SLATTACH is not set\n"
"CONFIG_TELNET=y\n"
"CONFIG_FEATURE_TELNET_TTYPE=y\n"
"CONFIG_FEATURE_TELNET_AUTOLOGIN=y\n"
"CONFIG_TELNETD=y\n"
"CONFIG_FEATURE_TELNETD_STANDALONE=y\n"
"CONFIG_TFTP=y\n"
"CONFIG_FEATURE_TFTP_GET=y\n"
"CONFIG_FEATURE_TFTP_PUT=y\n"
"# CONFIG_FEATURE_TFTP_BLOCKSIZE is not set\n"
"# CONFIG_DEBUG_TFTP is not set\n"
"# CONFIG_TRACEROUTE is not set\n"
"# CONFIG_FEATURE_TRACEROUTE_VERBOSE is not set\n"
"# CONFIG_FEATURE_TRACEROUTE_SOURCE_ROUTE is not set\n"
"# CONFIG_FEATURE_TRACEROUTE_USE_ICMP is not set\n"
"# CONFIG_APP_UDHCPD is not set\n"
"# CONFIG_APP_DHCPRELAY is not set\n"
"# CONFIG_APP_DUMPLEASES is not set\n"
"# CONFIG_FEATURE_UDHCPD_WRITE_LEASES_EARLY is not set\n"
"# CONFIG_APP_UDHCPC is not set\n"
"# CONFIG_FEATURE_UDHCP_DEBUG is not set\n"
"# CONFIG_FEATURE_RFC3397 is not set\n"
"# CONFIG_VCONFIG is not set\n"
"CONFIG_WGET=y\n"
"CONFIG_FEATURE_WGET_STATUSBAR=y\n"
"CONFIG_FEATURE_WGET_AUTHENTICATION=y\n"
"CONFIG_FEATURE_WGET_LONG_OPTIONS=y\n"
"# CONFIG_ZCIP is not set\n"
"CONFIG_FREE=y\n"
"# CONFIG_FUSER is not set\n"
"CONFIG_KILL=y\n"
"CONFIG_KILLALL=y\n"
"# CONFIG_KILLALL5 is not set\n"
"# CONFIG_NMETER is not set\n"
"CONFIG_PIDOF=y\n"
"# CONFIG_FEATURE_PIDOF_SINGLE is not set\n"
"# CONFIG_FEATURE_PIDOF_OMIT is not set\n"
"CONFIG_PS=y\n"
"CONFIG_FEATURE_PS_WIDE=y\n"
"# CONFIG_RENICE is not set\n"
"CONFIG_BB_SYSCTL=y\n"
"CONFIG_TOP=y\n"
"CONFIG_FEATURE_TOP_CPU_USAGE_PERCENTAGE=y\n"
"CONFIG_FEATURE_TOP_CPU_GLOBAL_PERCENTS=y\n"
"CONFIG_FEATURE_TOP_DECIMALS=y\n"
"CONFIG_UPTIME=y\n"
"# CONFIG_WATCH is not set\n"
"CONFIG_FEATURE_SH_IS_ASH=y\n"
"# CONFIG_FEATURE_SH_IS_HUSH is not set\n"
"# CONFIG_FEATURE_SH_IS_LASH is not set\n"
"# CONFIG_FEATURE_SH_IS_MSH is not set\n"
"# CONFIG_FEATURE_SH_IS_NONE is not set\n"
"CONFIG_ASH=y\n"
"CONFIG_ASH_JOB_CONTROL=y\n"
"CONFIG_ASH_READ_NCHARS=y\n"
"CONFIG_ASH_READ_TIMEOUT=y\n"
"CONFIG_ASH_ALIAS=y\n"
"CONFIG_ASH_MATH_SUPPORT=y\n"
"# CONFIG_ASH_MATH_SUPPORT_64 is not set\n"
"CONFIG_ASH_GETOPTS=y\n"
"CONFIG_ASH_BUILTIN_ECHO=y\n"
"CONFIG_ASH_BUILTIN_TEST=y\n"
"CONFIG_ASH_CMDCMD=y\n"
"# CONFIG_ASH_MAIL is not set\n"
"CONFIG_ASH_OPTIMIZE_FOR_SIZE=y\n"
"CONFIG_ASH_RANDOM_SUPPORT=y\n"
"CONFIG_ASH_EXPAND_PRMT=y\n"
"# CONFIG_HUSH is not set\n"
"# CONFIG_HUSH_HELP is not set\n"
"# CONFIG_HUSH_INTERACTIVE is not set\n"
"# CONFIG_HUSH_JOB is not set\n"
"# CONFIG_HUSH_TICK is not set\n"
"# CONFIG_HUSH_IF is not set\n"
"# CONFIG_HUSH_LOOPS is not set\n"
"# CONFIG_LASH is not set\n"
"# CONFIG_MSH is not set\n"
"# CONFIG_FEATURE_SH_EXTRA_QUIET is not set\n"
"# CONFIG_FEATURE_SH_STANDALONE is not set\n"
"# CONFIG_CTTYHACK is not set\n"
"CONFIG_SYSLOGD=y\n"
"CONFIG_FEATURE_ROTATE_LOGFILE=y\n"
"CONFIG_FEATURE_REMOTE_LOG=y\n"
"CONFIG_FEATURE_IPC_SYSLOG=y\n"
"CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE=16\n"
"CONFIG_LOGREAD=y\n"
"CONFIG_FEATURE_LOGREAD_REDUCED_LOCKING=y\n"
"CONFIG_KLOGD=y\n"
"# CONFIG_LOGGER is not set\n"
"# CONFIG_RUNSV is not set\n"
"# CONFIG_RUNSVDIR is not set\n"
"# CONFIG_SV is not set\n"
"# CONFIG_SVLOGD is not set\n"
"# CONFIG_CHPST is not set\n"
"# CONFIG_SETUIDGID is not set\n"
"# CONFIG_ENVUIDGID is not set\n"
"# CONFIG_ENVDIR is not set\n"
"# CONFIG_SOFTLIMIT is not set\n"
"# CONFIG_CHCON is not set\n"
"# CONFIG_FEATURE_CHCON_LONG_OPTIONS is not set\n"
"# CONFIG_GETENFORCE is not set\n"
"# CONFIG_GETSEBOOL is not set\n"
"# CONFIG_LOAD_POLICY is not set\n"
"# CONFIG_MATCHPATHCON is not set\n"
"# CONFIG_RESTORECON is not set\n"
"# CONFIG_RUNCON is not set\n"
"# CONFIG_FEATURE_RUNCON_LONG_OPTIONS is not set\n"
"# CONFIG_SELINUXENABLED is not set\n"
"# CONFIG_SETENFORCE is not set\n"
"# CONFIG_SETFILES is not set\n"
"# CONFIG_FEATURE_SETFILES_CHECK_OPTION is not set\n"
"# CONFIG_TCPSVD is not set\n"
"# CONFIG_UDPSVD is not set\n"
;
#endif /* _BBCONFIGOPTS_H */
