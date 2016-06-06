#!/usr/bin/perl

# Alexander Ho
# This program takes the AND/OR command and fields for the new entry from files printed out from the C++ program
# It creates an expression that will evaluate the AND/OR command using the eval() function
# The result will be written into a new file called result.txt

# Open all of the files we want to read and write from
my $fh_command;
my $fh_input;
my $fh_result;

open($fh_input, "< perl_input.txt") or die ("can't open file perl_input.txt");
open($fh_command, "< command.txt") or die ("can't open file command");
open($fh_result, "> result.txt") or die ("can't open file result");

# Create and initialize all of the fields in the entry
my $stb_id="";
my $title="";
my $provider="";
my $date="";
my $rev="";
my $view_time="";

# Read in the AND/OR command from command.txt and chomp the new line
my $command=<$fh_command>;   chomp($command);

# Replace the fields in the command to the variables created above
# Replace all of the AND/OR/= operations to the approriate format
$command =~ s/STB/\$stb_id/g;
$command =~ s/TITLE/\$title/g;
$command =~ s/PROVIDER/\$provider/g;
$command =~ s/DATE/\$date/g;
$command =~ s/REV/\$rev/g;
$command =~ s/VIEW_TIME/\$view_time/g;
$command =~ s/AND/\&\&/g;
$command =~ s/OR/\|\|/g;
$command =~ s/\=/ eq /g;

# Create the new command that will be passed into eval()
my $return_val="";
my $cmd = "if ( " . $command  . " ) { \$return_val = 1; } else { \$return_val=0; } ";

# Read the input file to get all of the fields for the new entry
while( $stb_id=<$fh_input> )
{
	chomp($stb_id);
	$title=<$fh_input>;         
        chomp($title);
	$provider=<$fh_input>;      
	chomp($provider);
	$date=<$fh_input>;          
	chomp($date);
	$rev=<$fh_input>;           
	chomp($rev);
	$view_time=<$fh_input>;     
	chomp($view_time);

	# Do the eval after it is done reading from the input file
	if( $stb_id ne "" )
	{
		eval $cmd ;
	}

	# Print out the results to the result.txt file
	if( $return_val == 1 )
	{
		print $fh_result "1\n" ;
	}
	else
	{
		print $fh_result "0\n";
	}
}

# Close all of the files we opened
close($fh_input);
close($fh_command);
close($fh_result);
exit 0;
