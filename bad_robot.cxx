/*
    Pretty minimal C++ solution to "toy robot" coding exercise:
        accept commands of form
            PLACE x y d
            MOVE
            LEFT
            RIGHT
            REPORT
        where d = N|NORTH|E|EAST|S|SOUTH|W|WEST
        and robot must remain within 5x5 square with origin at ( 0, 0 ).
    Reads from a supplied file name or stdin.
    Doesn't accept QUIT
    Doesn't accept lower-case commands.
    Doesn't accept commas.
    Does accept leading whitespace (hurrah).

    Not pretty. No classes. No debugging. One robot, represented as a global
    static struct. Not much meaningful division of labour. This is all barely
    worthy of being called C++, beyond a little use of STL. Not easily
    extensible or pluggable. Think yourself lucky I threw in some error
    handling. Well there are levels below which I will not let myself sink.

    An example of how to throw something together and not solve the problem
    nicely.
*/

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

enum Direction { Invalid, North, East, South, West };

typedef struct robot_s
{
    int xpos;
    int ypos;
    Direction direction;
    bool onTable;
} robot_t;

static robot_t theRobot;

static void readAndRunCommands ( FILE * fileStream );
static void runCommand ( const char * command );
static void reportPosition();
static string directionAsString ( const Direction & direction );
static Direction directionFromString ( const string & str );

extern int main ( int argc, char ** argv )
{
    try
    {
        // Initialise robot to a completely invalid state.
        theRobot.xpos = -1;             // }
        theRobot.ypos = -1;             // } pointless since not on table yet
        theRobot.direction = Invalid;   // }
        theRobot.onTable = false;

        // Read from supplied file or else stdin.
        if ( argc > 1 )
        {
            FILE * fileStream = fopen ( argv[1], "r" );
            if ( fileStream == 0 )
            {
                throw "Failed to read input file";
            }
            readAndRunCommands ( fileStream );
            fclose ( fileStream );
        }
        else
        {
            readAndRunCommands ( stdin );
        }
    }
    catch ( const string & error )
    {
        cerr << "Caught exception: " << error << endl;
        return 1;
    }
    catch ( const char * error )
    {
        cerr << "Caught exception: " << error << endl;
        return 1;
    }
    catch ( ... )
    {
        cerr << "Caught unknown exception" << endl;
        return 1;
    }
    return 0;
}

static void readAndRunCommands ( FILE * fileStream )
{
    // Loop until EOF, running each line in turn.
    for (;;)
    {
        char buffer[1024];
        if ( fgets ( buffer, 1024, fileStream ) == 0 ||
             feof ( fileStream ) != 0
           )
        {
            break;
        }
        runCommand ( buffer );
    }
}

static void runCommand ( const char * command )
{
    // Strip leading whitespace.
    while ( isspace ( *command ) )
    {
        ++command;
    }
    string commandStr ( command );

    // Trim trailing newline.
    if ( commandStr.length() > 0 )
    {
        commandStr = commandStr.substr ( 0, commandStr.length()-1 );
    }

    // Look for valid command. PLACE is special because that's the only one
    // that can affect the robot if it's not already on the table.
    if ( commandStr.substr ( 0, 6 ) == "PLACE " )
    {
        istringstream parser ( commandStr.substr ( 6 ) );
        int newXpos;
        int newYpos;
        string newDirectionToken;

        parser >> newXpos >> newYpos >> newDirectionToken;
        Direction newDirection = directionFromString ( newDirectionToken );
        if ( newDirection == Invalid )
        {
            cerr << "Invalid direction" << endl;
        }
        else if ( newXpos >= 0 && newXpos < 5 && newYpos >= 0 && newYpos < 5 )
        {
            theRobot.xpos = newXpos;
            theRobot.ypos = newYpos;
            theRobot.direction = newDirection;
            theRobot.onTable = true;
            reportPosition();
        }
        else
        {
            cerr << "Ignoring invalid PLACE co-ordinates" << endl;
        }
    }
    else if ( ! theRobot.onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        if ( commandStr == "MOVE" )
        {
            bool validMove = true;
            switch ( theRobot.direction )
            {
                case North:
                {
                    if ( theRobot.ypos < 4 )
                    {
                        ++theRobot.ypos;
                    }
                    else
                    {
                        validMove = false;
                    }
                    break;
                }
                case West:
                {
                    if ( theRobot.xpos > 0 )
                    {
                        --theRobot.xpos;
                    }
                    else
                    {
                        validMove = false;
                    }
                    break;
                }
                case South:
                {
                    if ( theRobot.ypos > 0 )
                    {
                        --theRobot.ypos;
                    }
                    else
                    {
                        validMove = false;
                    }
                    break;
                }
                case East:
                {
                    if ( theRobot.xpos < 4 )
                    {
                        ++theRobot.xpos;
                    }
                    else
                    {
                        validMove = false;
                    }
                    break;
                }
                case Invalid:
                {
                    cerr << "Attempt to move robot without placing it first" << endl;
                    break;
                }
                default:    // impossible, it's an enum
                {
                    throw "impossible enum value";
                    break;
                }
            }
            if ( ! validMove )
            {
                cerr << "Ignoring attempt to move robot off table" << endl;
            }

            reportPosition();
        }
        else if ( commandStr == "LEFT" )
        {
            theRobot.direction = ( theRobot.direction == North ) ? West :
                                 ( theRobot.direction == West )  ? South :
                                 ( theRobot.direction == South ) ? East :
                                 ( theRobot.direction == East )  ? North :
                                                                   Invalid;
            reportPosition();
        }
        else if ( commandStr == "RIGHT" )
        {
            theRobot.direction = ( theRobot.direction == North ) ? East :
                                 ( theRobot.direction == East )  ? South :
                                 ( theRobot.direction == South ) ? West :
                                 ( theRobot.direction == West )  ? North :
                                                                   Invalid;
            reportPosition();
        }
        else if ( commandStr == "REPORT" )
        {
            reportPosition();
        }
        else
        {
            cerr << "Ignoring invalid command " << endl;
        }
    }
}

static void reportPosition()
{
    if ( ! theRobot.onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        cout << "Robot is at x = " << theRobot.xpos << ", y = " << theRobot.ypos
             << ", facing " << directionAsString(theRobot.direction) << endl;
    }
}

static string directionAsString ( const Direction & direction )
{
    return ( direction == North ) ? "North" :
           ( direction == West )  ? "West" :
           ( direction == South ) ? "South" :
           ( direction == East )  ? "East" :
                                    "Invalid";
}

static Direction directionFromString ( const string & str )
{
    return ( str == "N" || str == "NORTH" ) ? North :
           ( str == "W" || str == "WEST" )  ? West :
           ( str == "S" || str == "SOUTH" ) ? South :
           ( str == "E" || str == "EAST" )  ? East :
                                              Invalid;
}
