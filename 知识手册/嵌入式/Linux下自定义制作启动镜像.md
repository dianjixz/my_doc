Ubuntu：genisoimage
Centos：mkisofs
    说明：上述两个命令分别对应不同的系统，当然也可能是另一条命令的链接。主要用于创建ISO 9660映像文件，可将指定的目录与文件做成ISO 9660格式的映像文件，以供刻录光盘。
    语法格式：genisoimage/mkisofs [参数]^
图片
执行
mkisofs \
或
genisoimage \
    -o ../wyh-test.iso \
    -input-charset utf-8 \
    -b isolinux/isolinux.bin \
    -c isolinux/boot.cat \
    -no-emul-boot \
    -boot-load-size 4 \
    -boot-info-table \
    -R -v -T -J -joliet-long \
    -V WYH \
    /var/lib/vz2/template/iso/iso-me/
图片
参数解析
-o FILE, -output FILE           设置输出文件名
-input-charset CHARSET          用于文件名转换的本地输入字符集
-b FILE, -eltorito-boot FILE    设置ElTorito启动镜像文件
-c FILE, -eltorito-catalog FILE 设置ElTorito引导目录文件
-no-emul-boot                   引导映像是“无模拟”映像
-boot-load-size #               设置负载扇区的数量
-boot-info-table                用信息表修补引导映像
-R, -rock                       生成Rock Ridge目录信息
-v, -verbose                    打印详细信息
-T, -translation-table          为不理解长文件名的系统生成翻译表
-J, -joliet                     生成Joliet目录信息
-joliet-long                    允许Joliet文件名为103个Unicode字符
-V ID, -volid ID                设置卷ID



图片
其他参数
Options:
  -nobak                      Do not include backup files
  -no-bak                     Do not include backup files
  -abstract FILE              Set Abstract filename
  -A ID, -appid ID            Set Application ID
  -biblio FILE                Set Bibliographic filename
  -cache-inodes               Cache inodes (needed to detect hard links)
  -no-cache-inodes            Do not cache inodes (if filesystem has no unique unides)
  -check-oldnames             Check all imported ISO9660 names from old session
  -check-session FILE         Check all ISO9660 names from previous session
  -copyright FILE             Set Copyright filename
  -debug                      Set debug flag
  -b FILE, -eltorito-boot FILE
                              Set El Torito boot image name
  -e FILE, -efi-boot FILE     Set EFI boot image name
  -eltorito-alt-boot          Start specifying alternative El Torito boot parameters
  -B FILES, -sparc-boot FILES Set sparc boot image names
  -sunx86-boot FILES          Set sunx86 boot image names
  -G FILE, -generic-boot FILE Set generic boot image name
  -sparc-label label text     Set sparc boot disk label
  -sunx86-label label text    Set sunx86 boot disk label
  -c FILE, -eltorito-catalog FILE
                              Set El Torito boot catalog name
  -C PARAMS, -cdrecord-params PARAMS
                              Magic paramters from cdrecord
  -d, -omit-period            Omit trailing periods from filenames (violates ISO9660)
  -dir-mode mode              Make the mode of all directories this mode.
  -D, -disable-deep-relocation
                              Disable deep directory relocation (violates ISO9660)
  -file-mode mode             Make the mode of all plain files this mode.
  -f, -follow-links           Follow symbolic links
  -gid gid                    Make the group owner of all files this gid.
  -graft-points               Allow to use graft points for filenames
  -root DIR                   Set root directory for all new files and directories
  -old-root DIR               Set root directory in previous session that is searched for files
  -help                       Print option help
  -hide GLOBFILE              Hide ISO9660/RR file
  -hide-list FILE             File with list of ISO9660/RR files to hide
  -hidden GLOBFILE            Set hidden attribute on ISO9660 file
  -hidden-list FILE           File with list of ISO9660 files with hidden attribute
  -hide-joliet GLOBFILE       Hide Joliet file
  -hide-joliet-list FILE      File with list of Joliet files to hide
  -hide-joliet-trans-tbl      Hide TRANS.TBL from Joliet tree
  -hide-rr-moved              Rename RR_MOVED to .rr_moved in Rock Ridge tree
  -gui                        Switch behaviour for GUI
  -i ADD_FILES                No longer supported
  -input-charset CHARSET      Local input charset for file name conversion
  -output-charset CHARSET     Output charset for file name conversion
  -iso-level LEVEL            Set ISO9660 conformance level (1..3) or 4 for ISO9660 version 2
  -J, -joliet                 Generate Joliet directory information
  -joliet-long                Allow Joliet file names to be 103 Unicode characters
  -jcharset CHARSET           Local charset for Joliet directory information
  -l, -full-iso9660-filenames Allow full 31 character filenames for ISO9660 names
  -max-iso9660-filenames      Allow 37 character filenames for ISO9660 names (violates ISO9660)
  -allow-limited-size         Allow different file sizes in ISO9660/UDF on large files
  -allow-leading-dots         Allow ISO9660 filenames to start with '.' (violates ISO9660)
  -ldots                      Allow ISO9660 filenames to start with '.' (violates ISO9660)
  -L, -allow-leading-dots     Allow ISO9660 filenames to start with '.' (violates ISO9660)
  -log-file LOG_FILE          Re-direct messages to LOG_FILE
  -m GLOBFILE, -exclude GLOBFILE
                              Exclude file name
  -exclude-list FILE          File with list of file names to exclude
  -pad                        Pad output to a multiple of 32k (default)
  -no-pad                     Do not pad output to a multiple of 32k
  -M FILE, -prev-session FILE Set path to previous session to merge
  -dev SCSIdev                Set path to previous session to merge
  -N, -omit-version-number    Omit version number from ISO9660 filename (violates ISO9660)
  -new-dir-mode mode          Mode used when creating new directories.
  -force-rr                   Inhibit automatic Rock Ridge detection for previous session
  -no-rr                      Inhibit reading of Rock Ridge attributes from previous session
  -no-split-symlink-components
                              Inhibit splitting symlink components
  -no-split-symlink-fields    Inhibit splitting symlink fields
  -path-list FILE             File with list of pathnames to process
  -p PREP, -preparer PREP     Set Volume preparer
  -print-size                 Print estimated filesystem size and exit
  -publisher PUB              Set Volume publisher
  -P PUB, -publisher PUB      Set Volume publisher
  -quiet                      Run quietly
  -r, -rational-rock          Generate rationalized Rock Ridge directory information
  -R, -rock                   Generate Rock Ridge directory information
  -s TYPE, -sectype TYPE      Set output sector type to e.g. data/xa1/raw
  -alpha-boot FILE            Set alpha boot image name (relative to image root)
  -hppa-cmdline CMDLINE       Set hppa boot command line (relative to image root)
  -hppa-kernel-32 FILE        Set hppa 32-bit image name (relative to image root)
  -hppa-kernel-64 FILE        Set hppa 64-bit image name (relative to image root)
  -hppa-bootloader FILE       Set hppa boot loader file name (relative to image root)
  -hppa-ramdisk FILE          Set hppa ramdisk file name (relative to image root)
  -mips-boot FILE             Set mips boot image name (relative to image root)
  -mipsel-boot FILE           Set mipsel boot image name (relative to image root)
  -jigdo-jigdo FILE           Produce a jigdo .jigdo file as well as the .iso
  -jigdo-template FILE        Produce a jigdo .template file as well as the .iso
  -jigdo-min-file-size SIZE   Minimum size for a file to be listed in the jigdo file
  -jigdo-force-md5 PATTERN    Pattern(s) where files MUST match an externally-supplied MD5sum
  -jigdo-exclude PATTERN      Pattern(s) to exclude from the jigdo file
  -jigdo-map PATTERN1=PATTERN2
                              Pattern(s) to map paths (e.g. Debian=/mirror/debian)
  -md5-list FILE              File containing MD5 sums of the files that should be checked
  -jigdo-template-compress ALGORITHM
                              Choose to use gzip or bzip2 compression for template data; default is gzip
  -checksum_algorithm_iso alg1,alg2,...
                              Specify the checksum types desired for the output image
  -checksum_algorithm_template alg1,alg2,...
                              Specify the checksum types desired for the output jigdo template
  -sort FILE                  Sort file content locations according to rules in FILE
  -split-output               Split output into files of approx. 1GB size
  -stream-file-name FILE_NAME Set the stream file ISO9660 name (incl. version)
  -stream-media-size #        Set the size of your CD media in sectors
  -sysid ID                   Set System ID
  -T, -translation-table      Generate translation tables for systems that don't understand long filenames
  -table-name TABLE_NAME      Translation table file name
  -ucs-level LEVEL            Set Joliet UCS level (1..3)
  -udf                        Generate UDF file system
  -dvd-video                  Generate DVD-Video compliant UDF file system
  -uid uid                    Make the owner of all files this uid.
  -U, -untranslated-filenames Allow Untranslated filenames (for HPUX & AIX - violates ISO9660). Forces -l, -d, -N, -allow-leading-dots, -relaxed-filenames, -allow-lowercase, -allow-multidot
  -relaxed-filenames          Allow 7 bit ASCII except lower case characters (violates ISO9660)
  -no-iso-translate           Do not translate illegal ISO characters '~', '-' and '#' (violates ISO9660)
  -allow-lowercase            Allow lower case characters in addition to the current character set (violates ISO9660)
  -allow-multidot             Allow more than one dot in filenames (e.g. .tar.gz) (violates ISO9660)
  -use-fileversion LEVEL      Use file version # from filesystem
  -v, -verbose                Verbose
  -version                    Print the current version
  -V ID, -volid ID            Set Volume ID
  -volset ID                  Set Volume set ID
  -volset-size #              Set Volume set size
  -volset-seqno #             Set Volume set sequence number
  -x FILE, -old-exclude FILE  Exclude file name(depreciated)
  -hard-disk-boot             Boot image is a hard disk image
  -no-emul-boot               Boot image is 'no emulation' image
  -no-boot                    Boot image is not bootable
  -boot-load-seg #            Set load segment for boot image
  -boot-load-size #           Set numbers of load sectors
  -boot-info-table            Patch boot image with info table
  -XA                         Generate XA directory attruibutes
  -xa                         Generate rationalized XA directory attruibutes
  -z, -transparent-compression
                              Enable transparent compression of files
  -hfs-type TYPE              Set HFS default TYPE
  -hfs-creator CREATOR        Set HFS default CREATOR
  -g, -apple                  Add Apple ISO9660 extensions
  -h, -hfs                    Create ISO9660/HFS hybrid
  -map MAPPING_FILE           Map file extensions to HFS TYPE/CREATOR
  -H MAPPING_FILE, -map MAPPING_FILE
                              Map file extensions to HFS TYPE/CREATOR
  -magic FILE                 Magic file for HFS TYPE/CREATOR
  -probe                      Probe all files for Apple/Unix file types
  -mac-name                   Use Macintosh name for ISO9660/Joliet/RockRidge file name
  -no-mac-files               Do not look for Unix/Mac files (depreciated)
  -boot-hfs-file FILE         Set HFS boot image name
  -part                       Generate HFS partition table
  -cluster-size SIZE          Cluster size for PC Exchange Macintosh files
  -auto FILE                  Set HFS AutoStart file name
  -no-desktop                 Do not create the HFS (empty) Desktop files
  -hide-hfs GLOBFILE          Hide HFS file
  -hide-hfs-list FILE         List of HFS files to hide
  -hfs-volid HFS_VOLID        Volume name for the HFS partition
  -icon-position              Keep HFS icon position
  -root-info FILE             finderinfo for root folder
  -input-hfs-charset CHARSET  Local input charset for HFS file name conversion
  -output-hfs-charset CHARSET Output charset for HFS file name conversion
  -hfs-unlock                 Leave HFS Volume unlocked
  -hfs-bless FOLDER_NAME      Name of Folder to be blessed
  -hfs-parms PARAMETERS       Comma separated list of HFS parameters
  -prep-boot FILE             PReP boot image file -- up to 4 are allowed
  -chrp-boot                  Add CHRP boot header
  --cap                       Look for AUFS CAP Macintosh files
  --netatalk                  Look for NETATALK Macintosh files
  --double                    Look for AppleDouble Macintosh files
  --ethershare                Look for Helios EtherShare Macintosh files
  --exchange                  Look for PC Exchange Macintosh files
  --sgi                       Look for SGI Macintosh files
  --macbin                    Look for MacBinary Macintosh files
  --single                    Look for AppleSingle Macintosh files
  --ushare                    Look for IPT UShare Macintosh files
  --xinet                     Look for XINET Macintosh files
  --dave                      Look for DAVE Macintosh files
  --sfm                       Look for SFM Macintosh files
  --osx-double                Look for MacOS X AppleDouble Macintosh files
  --osx-hfs                   Look for MacOS X HFS Macintosh files

