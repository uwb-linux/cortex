#!/usr/bin/perl -w
use strict;

use File::Basename;
use File::Spec;
use Getopt::Long;
use Benchmark;
use Cwd    qw( abs_path );
use FindBin qw($Bin);

my $cortex_dir;
BEGIN
{
    $cortex_dir = abs_path($0);
    $cortex_dir =~ s/scripts\/calling\/build_samples_parallel.pl//;
    push ( @INC, $cortex_dir."scripts/calling/");
}

use ConfigMgmt qw( get_from_config_if_undef );
use BasicUtils qw ( add_slash );
my $all_samples_index = "";
my $kmer = 31;
my $qthresh = 10;
my $bc="yes";
my $pd = "no";
my $mem_height = 0;
my $mem_width=0;
my $vcftools_dir = "";
my $stampy_bin="";
my $stampy_hash="";
my $list_ref="";
my $refbindir="";
my $ref_fa="";
my $num=-1;
my $genome_size=3000000;


my $outdir="";

&GetOptions(
    'num:i'                       =>\$num,
    'index:s'                     =>\$all_samples_index,
    'list_ref:s'                  =>\$list_ref,
    'refbindir:s'                 =>\$refbindir,
    'vcftools_dir:s'              =>\$vcftools_dir,
    'out_dir:s'                   =>\$outdir,
    'stampy_bin:s'                =>\$stampy_bin,
    'stampy_hash:s'               =>\$stampy_hash,
    'bc:s'                        =>\$bc,
    'pd:s'                        =>\$pd,
    'kmer:i'                      =>\$kmer,
    'mem_height:i'                =>\$mem_height,
    'mem_width:i'                 =>\$mem_width,
    'genome_size:i'                 =>\$genome_size,
    'qthresh:i'                   =>\$qthresh,
    );



$outdir = BasicUtils::add_slash($outdir);
my $index_dir =$outdir."indexes/";
my $prev_config = $outdir."config.prep.txt";
my $this_config = $outdir."config.par.txt";
my $ref_binary;
($mem_height, $mem_width) 
    = check_args($num, $index_dir, $mem_height, $mem_width, 
		 $genome_size, $kmer);



### User does not need to enter these arguments, wil be carried over
### in the config file generated by prepare.pl
($vcftools_dir, $stampy_bin, 
 $stampy_hash,$list_ref, 
 $ref_fa, $refbindir, $genome_size) = 
    get_args_from_config_if_undefined($vcftools_dir, $stampy_bin, 
				      $stampy_hash,$list_ref, 
				      $ref_fa, $refbindir, $genome_size, $prev_config);

$ref_binary = check_args_second($vcftools_dir, $stampy_bin, 
			       $stampy_hash,$list_ref, 
			       $ref_fa, $refbindir, $kmer);  


##remember many par.pl processes run in parallel, can't all write to same file
if ($num==1)
{
    open(CONFIG, ">".$this_config)||die("Unable to create $this_config\n");
    print CONFIG "vcftools_dir\t$vcftools_dir\n";
    print CONFIG "kmer\t$kmer\n";
    print CONFIG "ref_binary\t$ref_binary\n";
    print CONFIG "genome_size\t$genome_size\n";
    print CONFIG "mem_height\t$mem_height\n";
    print CONFIG "mem_width\t$mem_width\n";
    close(CONFIG);
}

my $index = $index_dir."index_".$num;
my $c1 = "head -n $num $all_samples_index  | tail -n 1 > $index";
qx{$c1};
open(F, $index)||die();
my $line= <F>;
chomp $line;
my @sp = split(/\t/, $line);
my $sample = $sp[0];
my $odir = $outdir.$sample.'/';
my $c2 = "mkdir -p $odir";
qx{$c2};
my $log = $odir."log_bc.".$sample;
print "$log\n";

my $cmd ="perl $cortex_dir"."scripts/calling/run_calls.pl --fastaq_index $index --first_kmer $kmer --auto_cleaning yes --bc $bc --pd $pd --outdir $odir --ploidy 1 --genome_size $genome_size --mem_height $mem_height --mem_width $mem_width --qthresh $qthresh --vcftools_dir $vcftools_dir  --do_union yes --logfile $log,f --workflow independent --ref CoordinatesAndInCalling --list_ref $list_ref --refbindir $refbindir --stampy_bin $stampy_bin --stampy_hash $stampy_hash --outvcf $sample ";
qx{$cmd};



sub get_args_from_config_if_undefined
{
    my ($vcft, $st_bin, $st_hash, 
	$list_r, $r_fa, $rbindir, $g_size, $conf) = @_;


    ## My current system does allow 
    ## you to have no config file, am ignorning
    ## the error/return values
    my $err;
    ($err, $vcft)   = get_from_config_if_undef($vcft, "vcftools_dir", $conf); 
    ($err, $st_bin) = get_from_config_if_undef($st_bin, "stampy_bin", $conf);
    ($err, $st_hash)= get_from_config_if_undef($st_hash, "stampy_hash", $conf);
    ($err, $list_r) = get_from_config_if_undef($list_r, "list_ref", $conf);
    ($err, $r_fa)   = get_from_config_if_undef( $r_fa, "ref_fa", $conf);
    ($err, $rbindir)= get_from_config_if_undef( $rbindir, "refbindir", $conf);
    ($err, $g_size) = get_from_config_if_undef( $g_size, "genome_size", $conf);

    return ($vcft, $st_bin, $st_hash, 
	    $list_r, $r_fa, $rbindir, $g_size);

}

sub check_args
{
    my ($n, $i_dir, $mh, $mw, $g, $km) = @_;
    if ($km % 2==0)
    {
	die("Kmer must be an odd number\n");
    }
    elsif ($km<10) 
    {
	die("I'm just going to stop this being run for k<10, can't believe it will be useful - typo?\n");
    }
    if ($n==-1)
    {
	die("--num is a mandatory argument, specifies which sample from the INDEX file to run\n");
    }
    if (!(-d $i_dir))
    {
	my $c1 = "mkdir $i_dir";
	qx{$c1};
    }	
    if ($g==0)
    {
	die("You must enter a genome size (in bp) using --genome_size. An estimate is fine. This will be used to get memory use parameters mem height/width (if you do not enter them) and later on will be used for likelihood calculations\n");
    }
    if ( ($mh==0) ||  ($mw==0))
    {
	$mw=100;

	## 2^height * width = 4g
	## => height = log_2(4g/width)
	$mh = int(log(4*$g/100)/log(2) +0.5);
    }


    return ($mh, $mw);
}


sub check_args_second
{
    my ($vcftools_dir, $stampy_bin, $stampy_hash,$list_ref, 
	$ref_fa, $refbindir, $km) = @_;

    if ($vcftools_dir eq "")
    {
	die("Either you should use prepare.pl, which would leave a config file specifying vcftools_dir (which it appears you have not), or you must specify --vcftools_dir.\n");
    }
    if ($stampy_bin eq "")
    {
	die("Either you should use prepare.pl, which would leave a config file specifying stampy_bin (which it appears you have not), or you must specify --stampy_bin.\n");
    }
    if ($stampy_hash eq "")
    {
	die("Either you should use prepare.pl, which would leave a config file specifying stampy_hash (which it appears you have not), or you must specify --stampy_hash.\n");
    }
    if ($list_ref eq "")
    {
	die("Either you should use prepare.pl, which would leave a config file specifying list_ref (which it appears you have not), or you must specify --list_ref.\n");
    }
    if ($ref_fa eq "")
    {
	die("Either you should use prepare.pl, which would leave a config file specifying ref_fa (which it appears you have not), or you must specify --ref_fa.\n");
    }
    my $c = "cat $list_ref";
    my $cr = qx{$c};
    chomp $cr;
    if (abs_path($cr) ne abs_path($ref_fa))
    {
	die("The reference fasta list $list_ref does not correspond to (refer to) the ref fasta\n");
    }

    if (!(-d $refbindir))
    {
	die("The specified directory $refbindir does not exist\n");
    }

    my @files = glob($refbindir."/*k".$km.".ctx");
    if (scalar @files==0)
    {
	die("The reference binary directory $refbindir does not contain any files with names ending k$kmer".".ctx; \neither you have not followed the naming convention we are asking for, \nor there is no Cortex binary graph file of the reference genome in that directory\n");
    }
    elsif (scalar @files>1)
    {
	die("There is more than one file in the reference binary directory $refbindir with name ending k$kmer".".ctx, so this script can't work out which is the reference binary\n");
    }
    return $files[0];
}
