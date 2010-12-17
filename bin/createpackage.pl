#!/usr/bin/perl
#############################################################################
##
## Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
## All rights reserved.
## Contact: Nokia Corporation (qt-info@nokia.com)
##
## This file is part of the S60 port of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:LGPL$
## Commercial Usage
## Licensees holding valid Qt Commercial licenses may use this file in
## accordance with the Qt Commercial License Agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and Nokia.
##
## GNU Lesser General Public License Usage
## Alternatively, this file may be used under the terms of the GNU Lesser
## General Public License version 2.1 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU Lesser General Public License version 2.1 requirements
## will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## In addition, as a special exception, Nokia gives you certain additional
## rights.  These rights are described in the Nokia Qt LGPL Exception
## version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU General Public License version 3.0 requirements will be
## met: http://www.gnu.org/copyleft/gpl.html.
##
## If you have questions regarding the use of this file, please contact
## Nokia at qt-info@nokia.com.
## $QT_END_LICENSE$
##
#############################################################################

############################################################################################
#
# Convenience script for creating signed packages you can install on your phone.
#
############################################################################################

use strict;

# use a command-line parsing module
use Getopt::Long;
# Use file name parsing module
use File::Basename;
# Use File::Spec services mainly rel2abs
use File::Spec;
# Use File::Path - to make stub sis target directory
use File::Path;
# use CWD abs_bath, which is exported only on request
use Cwd 'abs_path';


sub Usage() {
    print <<ENDUSAGESTRING;

==============================================================================================
Convenience script for creating signed packages you can install on your phone.

Usage: createpackage.pl [options] templatepkg [target]-[platform] [certificate key [passphrase]]

Where supported options are as follows:
     [-i|install]            = Install the package right away using PC suite.
     [-p|preprocess]         = Only preprocess the template .pkg file.
     [-c|certfile <file>]    = The file containing certificate information for signing.
                               The file can have several certificates, each specified in
                               separate line. The certificate, key and passphrase in line
                               must be ';' separated. Lines starting with '#' are treated
                               as a comments. Also empty lines are ignored. The paths in
                               <file> can be absolute or relative to <file>.
     [-u|unsigned]           = Preserves the unsigned package.
     [-o|only-unsigned]      = Creates only unsigned package.
     [-s|stub]               = Generates stub sis for ROM.
     [-n|sisname <name>]     = Specifies the final sis name.
     [-g|gcce-is-armv5]      = Convert gcce platform to armv5.
Where parameters are as follows:
     templatepkg             = Name of .pkg file template
     target                  = Either debug or release
     platform                = One of the supported platform
                               winscw | gcce | armv5 | armv6 | armv7
                               Note that when packaging binaries built using gcce and symbian-sbsv2
                               mkspec, armv5 must be used for platform instead of gcce.
     certificate             = The certificate file used for signing
     key                     = The certificate's private key file
     passphrase              = The passphrase of the certificate's private key file

Example:
     createpackage.pl fluidlauncher_template.pkg release-armv5

Example with certfile:
     createpackage.pl -c=mycerts.txt fluidlauncher_template.pkg release-armv5

     Content of 'mycerts.txt' must be something like this:
        # This is comment line, also the empty lines are ignored
        rd.cer;rd-key.pem
        .\\cert\\mycert.cer;.\\cert\\mykey.key;yourpassword
        X:\\QtS60\\s60installs\\selfsigned.cer;X:\\QtS60\\s60installs\\selfsigned.key

If no certificate and key files are provided, either a RnD certificate or
a self-signed certificate from QtDir\\src\\s60installs directory is used.
In the latter case the resulting package will also be automatically patched
using patch_capabilities.pl script, which makes it unsuitable for distribution.
Always specify certificates explicitly if you wish to distribute your package.

==============================================================================================

ENDUSAGESTRING

    exit();
}

# Read given options
my $install = "";
my $preprocessonly = "";
my $certfile = "";
my $preserveUnsigned = "";
my $stub = "";
my $signed_sis_name = "";
my $onlyUnsigned = "";
my $convertGcce = "";

unless (GetOptions('i|install' => \$install,
                   'p|preprocess' => \$preprocessonly,
                   'c|certfile=s' => \$certfile,
                   'u|unsigned' => \$preserveUnsigned,
                   'o|only-unsigned' => \$onlyUnsigned,
                   's|stub' => \$stub,
                   'n|sisname=s' => \$signed_sis_name,
                   'g|gcce-is-armv5' => \$convertGcce,)) {
    Usage();
}

my $epocroot = $ENV{EPOCROOT};
$epocroot =~ s,[\\/]$,,x;

my $certfilepath = abs_path(dirname($certfile));

# Read params to variables
my $templatepkg = $ARGV[0];
my $targetplatform = lc $ARGV[1];

if ($targetplatform eq "") {
    $targetplatform = "-";
}

my @tmpvalues = split('-', $targetplatform);
my $target;
$target = $tmpvalues[0] or $target = "";
my $platform;
$platform = $tmpvalues[1] or $platform = "";

if ($platform =~ m/^gcce$/i) {
    if (($convertGcce ne "")) {
        $platform = "armv5";
    } elsif ($ENV{SBS_HOME}) {
        # Print a informative note in case suspected misuse is detected.
        print "\nNote: You should use armv5 as platform or specify -g parameter to convert platform\n";
        print "      when packaging gcce binaries built using symbian-sbsv2 mkspec.\n\n";
    }
}

# Convert visual target to real target (debug->udeb and release->urel)
$target =~ s/debug/udeb/i;
$target =~ s/release/urel/i;

my $certificate;
$certificate = $ARGV[2] or $certificate = "";
my $key;
$key = $ARGV[3] or $key = "";
my $passphrase;
$passphrase = $ARGV[4] or $passphrase = "";

# Generate output pkg basename (i.e. file name without extension)
my $pkgoutputbasename = $templatepkg;
my $preservePkgOutput = "";
$pkgoutputbasename =~ s/_template/_$targetplatform/g;
$pkgoutputbasename =~ s/_installer\.pkg/_installer___temp\.pkg/g;
if ($pkgoutputbasename eq $templatepkg) {
    $preservePkgOutput = "1";
}
$pkgoutputbasename =~ s/\.pkg//g;

# Store output file names to variables
my $pkgoutput = $pkgoutputbasename.".pkg";
my $sisoutputbasename;
if ($signed_sis_name eq "") {
    $sisoutputbasename = $pkgoutputbasename;
    $sisoutputbasename =~ s/_$targetplatform//g;
    $sisoutputbasename =~ s/_installer___temp/_installer/g;
    $signed_sis_name = $sisoutputbasename.".sis";
} else {
    $sisoutputbasename = $signed_sis_name;
    if ($sisoutputbasename =~ m/(\.sis$|\.sisx$)/i) {
        $sisoutputbasename =~ s/$1//i;
    } else {
        $signed_sis_name = $signed_sis_name.".sis";
    }
}

my $installer_unsigned_app_sis_name = "";
my $installer_app_sis_name = "";

if ($templatepkg =~ m/_installer\.pkg$/i && $onlyUnsigned) {
    $installer_unsigned_app_sis_name = $templatepkg;
    $installer_unsigned_app_sis_name =~ s/_installer.pkg$/_unsigned.sis/i;
    $installer_app_sis_name = $installer_unsigned_app_sis_name;
    $installer_app_sis_name =~ s/_unsigned.sis$/.sis/;
}

my $unsigned_sis_name = $sisoutputbasename."_unsigned.sis";
my $stub_sis_name = $sisoutputbasename.".sis";

# Store some utility variables
my $scriptpath = dirname(__FILE__);
my $certtext = $certificate;
# certificates are one step up in hierarchy
my $certpath = File::Spec->catdir($scriptpath, File::Spec->updir(), "src/s60installs/");

# Check some pre-conditions and print error messages if needed.
unless (length($templatepkg)) {
    print "\nError: Template PKG filename is not defined!\n";
    Usage();
}

# Check template exist
stat($templatepkg);
unless( -e _ ) {
    print "\nError: Package description file '$templatepkg' does not exist!\n";
    Usage();
}

# Check certifcate preconditions and set default certificate variables if needed
if (length($certificate)) {
    unless(length($key)) {
        print "\nError: Custom certificate key file parameter missing.!\n";
        Usage();
    }
} else {
    #If no certificate is given, check default options
    $certtext = "RnD";
    $certificate = File::Spec->catfile($certpath, "rd.cer");
    $key = File::Spec->catfile($certpath, "rd-key.pem");

    stat($certificate);
    unless( -e _ ) {
        $certtext = "Self Signed";
        $certificate = File::Spec->catfile($certpath, "selfsigned.cer");
        $key = File::Spec->catfile($certpath, "selfsigned.key");
    }
}

# Read the certificates from file to two dimensional array
my @certificates;
if (length($certfile)) {
    open CERTFILE, "<$certfile" or die $!;
    while(<CERTFILE>){
        s/#.*//;                            # ignore comments by erasing them
        next if /^(\s)*$/;                  # skip blank lines
        chomp;                              # remove trailing newline characters
        my @certinfo = split(';', $_);      # split row to certinfo

        # Trim spaces
        for(@certinfo) {
            s/^\s+//;
            s/\s+$//;
        }

        # Do some validation
        unless(scalar(@certinfo) >= 2 && scalar(@certinfo) <= 3 && length($certinfo[0]) && length($certinfo[1]) ) {
            print "\nError: $certfile line '$_' does not contain valid information!\n";
            Usage();
        }

        push @certificates, [@certinfo];    # push data to two dimensional array
    }
}

# Remove any existing .sis packages
unlink $unsigned_sis_name;
if (!$onlyUnsigned) {
    unlink $signed_sis_name;
}
if (!$preservePkgOutput) {
    unlink $pkgoutput;
}

# Preprocess PKG

local $/;
# read template file
open( TEMPLATE, $templatepkg) or die "Error '$templatepkg': $!\n";
$_=<TEMPLATE>;
close (TEMPLATE);

# If the pkg file does not contain macros, there is no need for platform or target.
if (m/\$\(PLATFORM\)/) {
    unless (length($platform) && length($target)) {
        print "\nError: Platform or target is not defined!\n";
        Usage();
    }
}

# replace the PKG variables
s/\$\(PLATFORM\)/$platform/gm;
s/\$\(TARGET\)/$target/gm;

if ($installer_unsigned_app_sis_name ne "") {
    s/$installer_app_sis_name\"/$installer_unsigned_app_sis_name\"/;
}

#write the output
open( OUTPUT, ">$pkgoutput" ) or die "Error '$pkgoutput' $!\n";
print OUTPUT $_;
close OUTPUT;

if ($preprocessonly) {
    exit;
}

if($stub) {
    if(!($epocroot)) { die("EPOCROOT must be set to create stub sis files"); }
    my $systeminstall = "$epocroot/epoc32/data/z/system/install";
    mkpath($systeminstall);
    my $stub_sis_name = $systeminstall."/".$stub_sis_name;
    # Create stub SIS.
    system ("$epocroot/epoc32/tools/makesis -s $pkgoutput $stub_sis_name");
} else {
    if ($certtext eq "Self Signed"
        && !@certificates
        && $templatepkg !~ m/_installer\.pkg$/i
        && !$onlyUnsigned) {
        print("Auto-patching capabilities for self signed package.\n");
        my $patch_capabilities = File::Spec->catfile(dirname($0), "patch_capabilities");
        system ("$patch_capabilities $pkgoutput");
    }

    # Create SIS.
    # The 'and' is because system uses 0 to indicate success.
    if($epocroot) {
        system ("$epocroot/epoc32/tools/makesis $pkgoutput $unsigned_sis_name") and die ("makesis failed");
    } else {
        system ("makesis $pkgoutput $unsigned_sis_name") and die ("makesis failed");
    }
    print("\n");

    my $targetInsert = "";
    if ($targetplatform ne "-") {
        $targetInsert = " for $targetplatform";
    }

    if ($onlyUnsigned) {
        stat($unsigned_sis_name);
        if( -e _ ) {
            print ("Successfully created unsigned package ${unsigned_sis_name}${targetInsert}!\n");
        } else {
            print ("\nUnsigned package creation failed!\n");
        }

        if (!$preservePkgOutput) {
            unlink $pkgoutput;
        }
        print ("\n");
        exit;
    }

    # Sign SIS with certificate info given as an argument.
    my $relcert = File::Spec->abs2rel($certificate);
    my $relkey = File::Spec->abs2rel($key);
    # The 'and' is because system uses 0 to indicate success.
    system ("signsis $unsigned_sis_name $signed_sis_name $relcert $relkey $passphrase") and die ("signsis failed");

    # Check if creating signed SIS Succeeded
    stat($signed_sis_name);
    if( -e _ ) {
        print ("Successfully created signed package ${signed_sis_name}${targetInsert} using certificate: $certtext!\n");

        # Sign with additional certificates & keys
        for my $row ( @certificates ) {
            # Get certificate absolute file names, relative paths are relative to certfilepath
            my $relcert = File::Spec->abs2rel(File::Spec->rel2abs( $row->[0], $certfilepath));
            my $relkey = File::Spec->abs2rel(File::Spec->rel2abs( $row->[1], $certfilepath));

            system ("signsis $signed_sis_name $signed_sis_name $relcert $relkey $row->[2]");
            print ("\tAdditionally signed the SIS with certificate: $row->[0]!\n");
        }

        # remove temporary pkg and unsigned sis
        if (!$preservePkgOutput) {
            unlink $pkgoutput;
        }
        if (!$preserveUnsigned) {
            unlink $unsigned_sis_name;
        }

        # Install the sis if requested
        if ($install) {
            print ("\nInstalling $signed_sis_name...\n");
            system ("$signed_sis_name");
        }
    } else {
        # Lets leave the generated PKG for problem solving purposes
        print ("\nSIS creation failed!\n");
    }
    print ("\n");
}

#end of file
