#This script updates the mmwave profile configuration files
#for the mmwave demo. Valid only for xwr14xx scripts.
#syntax: perl mmwDemo_xwr14xx_update_config.pl <your_cfg_file> 
#output file: <your_cfg_file>_updated

die "syntax: $0   <Input cfg file> \n" if ($#ARGV != 0);
$inputFile = $ARGV[0]; 
$scriptName = $0;
open INPUT, $inputFile or die "Can't open $inputFile\n";
$outputFile = ">" . $inputFile . "_updated";
open OUTPUT, $outputFile or die "\nCan't create file to update config: " . $outputFile."\n";

$scriptVersion = "2.0";

#Updating the config file to work with visualizer given by the version below
# visualizerVersion = v1.v2.v3
$v1 = 2;
$v2 = 0;
$v3 = 0;
$visualizerVersion = $v1.".".$v2.".".$v3;

#####################################################################
#If you are adding a new command, just add the command line
#in the newCommandsArray below and you are done. 
#####################################################################
@newCommandsArray = 
(
"clutterRemoval 0",
"calibDcRangeSig 0 -5 8 256",
"compRangeBiasAndRxChanPhase 0.0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0",
"measureRangeBiasAndRxChanPhase 0 1.5 0.2",
"CQRxSatMonitor 0 3 4 13 0",
"CQSigImgMonitor 0 13 4",
"analogMonitor 0 0",
"lowPower 0 0"
);

####################### DO NOT CHANGE THE SCRIPT BEYOND THIS POINT ##################################################

#Pre process new commands
@newCommandCmd = ();
@newCommandFoundFlag = ();
for($i=0;$i<scalar(@newCommandsArray);$i++)
{
    @f1 = split(/ /, $newCommandsArray[$i]);
    $newCommandCmd[$i] = $f1[0];
    $newCommandFoundFlag[$i] = 0;
}

#warning banner
$warningBanner = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

#Comment to be added when a new command is inserted in the script
$newCmdMsg = "\%Inserting new mandatory command. Check users guide for details.\n";

#String appended to the begining of the command
$update_string = " -1";

#first run through the file to check for missing commands introduced in the latest release
#and to check if this is a multiple start/stop scenario
@startPositions=();
@stopPositions=();
$missingCmdFlag = 0;
$lineCount = 0;
while (<INPUT>) 
{
    $lineCount++;
    $i = $_;
    if (index($i,"sensorStart")>=0 ) 
    {
       push @startPositions, $lineCount;
       next;       
    }
    
    if (index($i,"sensorStop")>=0 ) 
    {
       push @stopPositions, $lineCount;
       next;       
    }
    
    #Set cmd found flag if command is present
    for($j=0;$j<scalar(@newCommandsArray);$j++)
    {
        if(index($i,$newCommandCmd[$j])>=0 ) 
        {
           $newCommandFoundFlag[$j] = 1;
           last;       
        }
    }
    
}

$warningMsg = $warningBanner."The following mandatory command(s) was(were) added to the output script. Check the users guide for details.\n";
for($j=0;$j<scalar(@newCommandsArray);$j++)
{
    if($newCommandFoundFlag[$j] == 0) 
    {
        $missingCmdFlag = 1;
        $warningMsg = $warningMsg."Command: ".$newCommandCmd[$j]."\n";
    }
}
$warningMsg = $warningMsg.$warningBanner;

seek INPUT, 0, 0;

#create output file
print OUTPUT "\%This file was updated by script ",$scriptName," version:",$scriptVersion,"\n";
print OUTPUT "\%This file is compatible with Visualizer Version:",$visualizerVersion,"\n";
$lineCount = 0;

if((scalar(@startPositions)==1) && (scalar(@startPositions)==1))
{
    #This is the normal profile configuration with one start and one stop
    updateConfig($stopPositions[0],$startPositions[0]);
}
elsif ((scalar(@startPositions)==6) && (scalar(@startPositions)==6))
{
    #This is the start/stop test
    #update first configuration
    updateConfig($stopPositions[0],$startPositions[0]);
    #update second configuration
    updateConfig($stopPositions[3],$startPositions[3]);
    #copy remaining lines of the test after the second configuration
    while (<INPUT>) 
    {
        print OUTPUT $_;
    }    
}
else
{
    die "\n!!! Error. This config file has more than one sensorStart and/or sensorStop\n"; 
}

sub updateConfig()
{
    $sensorStopLine  = $_[0];
    $sensorStartLine = $_[1];
    
    while (<INPUT>) 
    {
        #print "$_\n";
        $i = $_;
        $lineCount++;
        if($lineCount > $sensorStartLine)
        {
            #print this line (because it has been read already) and exit the function
            print OUTPUT $i;        
            return;
        }
        if($lineCount < $sensorStopLine)
        {
            #Nothing should be done before we reach sensorStop. Just copy line to output.
            print OUTPUT $i;        
            next;
        }
        
        if($lineCount == $sensorStartLine)
        {
            #Lets place here the missing commands, just before sensorStart cmd
            for($j=0;$j<scalar(@newCommandsArray);$j++)
            {
                if($newCommandFoundFlag[$j] == 0) 
                {
                    print OUTPUT $newCmdMsg .$newCommandsArray[$j]."\n";
                }
            }

        }
        # Just copy line to output.
        print OUTPUT $i;        
    }
}

close (INPUT);
close (OUTPUT);
if($missingCmdFlag == 1)
{
    die $warningMsg;
}

