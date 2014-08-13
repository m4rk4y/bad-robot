/*
    Pretty minimal C++ solution to "toy robot" coding exercise:
        accept commands of form
            PLACE x,y,d
            MOVE
            LEFT
            RIGHT
            REPORT
        where d = N|NORTH|E|EAST|S|SOUTH|W|WEST
        and robot must remain within 5x5 square with origin at ( 0, 0 ).
    Reads from a supplied file name or stdin.
    Doesn't accept QUIT
    Doesn't accept lower-case commands.

    Not pretty. No debugging. One robot.
    Not easily extensible or pluggable.

    An example of how to throw something together and not solve the problem
    really nicely.
*/

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

enum Direction { Invalid, North, East, South, West };

class Robot
{
    public:
        Robot();
        void readAndRunCommands ( FILE * fileStream );
        void runCommand ( const string & command );
        void place ( int xpos, int ypos, Direction direction );
        void move();
        void left();
        void right();
        void report() const;
    private:
        int m_xpos;
        int m_ypos;
        Direction m_direction;
        bool m_onTable;
};

static string directionAsString ( const Direction & direction );
static Direction directionFromString ( const string & str );

extern int main ( int argc, char ** argv )
{
    try
    {
        Robot robot;

        // Read from supplied file or else stdin.
        if ( argc > 1 )
        {
            FILE * fileStream = fopen ( argv[1], "r" );
            if ( fileStream == 0 )
            {
                throw "Failed to read input file";
            }
            robot.readAndRunCommands ( fileStream );
            fclose ( fileStream );
        }
        else
        {
            robot.readAndRunCommands ( stdin );
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

Robot::Robot()
 : m_xpos ( -1 ),           // }
   m_ypos ( -1 ),           // } pointless since not on table yet
   m_direction ( Invalid ), // }
   m_onTable ( false )
{
}

void Robot::readAndRunCommands ( FILE * fileStream )
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
        try
        {
            // Trim trailing newline.
            buffer[strlen(buffer)-1] = '\0';
            runCommand ( buffer );
        }
        catch ( const exception & error )
        {
            cerr << "Caught exception: " << error.what() << endl;
        }
    }
}

void raiseInvalidPlaceCommand ( const string & str )
{
    stringstream messageStream;
    messageStream << "Invalid PLACE command: " << str;
    throw exception ( messageStream.str().c_str() );
}

void Robot::runCommand ( const string & command )
{

    istringstream parser ( command );
    string verb;
    parser >> verb;
    if ( verb == "PLACE" )
    {
        // Can't use STL parser to separate comma-delimited tokens :-(
        // Do it by steam instead.
        // Also allow for istringstream having skipped leading whitespace, so
        // "PLACE" might not be at the beginning of the command.
        size_t placePos = command.find ( "PLACE " );
        size_t placeEndPos = placePos + 6;
        size_t comma1Pos = command.find_first_of ( ",", placeEndPos+1 );
        if ( comma1Pos == string::npos )
        {
            raiseInvalidPlaceCommand ( command );
        }
        size_t comma2Pos = command.find_first_of ( ",", comma1Pos+1 );
        if ( comma2Pos == string::npos )
        {
            raiseInvalidPlaceCommand ( command );
        }
        // Example:
        // command = "    PLACE 12,100,N"
        // => placePos = 4
        //    placeEndPos = 10
        // => comma1Pos = 12
        //    comma2Pos = 16
        //    x = [10..11]
        //    y = [13..15]
        //    d = [17...]
        // 6 = strlen ( "PLACE " )
        int newXpos = atoi ( command.substr ( placeEndPos, comma1Pos-placeEndPos ).c_str() );
        int newYpos = atoi ( command.substr ( comma1Pos+1, comma2Pos-comma1Pos-1 ).c_str() );
        string newDirectionToken ( command.substr ( comma2Pos+1 ) );
        Direction newDirection = directionFromString ( newDirectionToken );
        if ( newDirection == Invalid )
        {
            stringstream messageStream;
            messageStream << "Invalid direction: " << newDirectionToken;
            throw exception ( messageStream.str().c_str() );
        }
        place ( newXpos, newYpos, newDirection );
    }
    else if ( verb == "MOVE" )
    {
        move();
    }
    else if ( verb == "LEFT" )
    {
        left();
    }
    else if ( verb == "RIGHT" )
    {
        right();
    }
    else if ( verb == "REPORT" )
    {
        report();
    }
    else
    {
        stringstream messageStream;
        messageStream << "Invalid command: " << command;
        throw exception ( messageStream.str().c_str() );
    }
}

void Robot::place ( int newXpos, int newYpos, Direction newDirection )
{
    if ( newXpos >= 0 && newXpos < 5 && newYpos >= 0 && newYpos < 5 )
    {
        m_xpos = newXpos;
        m_ypos = newYpos;
        m_direction = newDirection;
        m_onTable = true;
        report();
    }
    else
    {
        stringstream messageStream;
        messageStream << "Invalid PLACE co-ordinates: "
                      << newXpos << " " << newYpos;
        throw exception ( messageStream.str().c_str() );
    }
}

void Robot::move()
{
    if ( ! m_onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        bool validMove = true;
        switch ( m_direction )
        {
            case North:
            {
                if ( m_ypos < 4 )
                {
                    ++m_ypos;
                }
                else
                {
                    validMove = false;
                }
                break;
            }
            case West:
            {
                if ( m_xpos > 0 )
                {
                    --m_xpos;
                }
                else
                {
                    validMove = false;
                }
                break;
            }
            case South:
            {
                if ( m_ypos > 0 )
                {
                    --m_ypos;
                }
                else
                {
                    validMove = false;
                }
                break;
            }
            case East:
            {
                if ( m_xpos < 4 )
                {
                    ++m_xpos;
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

        report();
    }
}

void Robot::left()
{
    if ( ! m_onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        m_direction = ( m_direction == North ) ? West :
                      ( m_direction == West )  ? South :
                      ( m_direction == South ) ? East :
                      ( m_direction == East )  ? North :
                                                 Invalid;
        report();
    }
}

void Robot::right()
{
    if ( ! m_onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        m_direction = ( m_direction == North ) ? East :
                      ( m_direction == East )  ? South :
                      ( m_direction == South ) ? West :
                      ( m_direction == West )  ? North :
                                                 Invalid;
        report();
    }
}

void Robot::report() const
{
    if ( ! m_onTable )
    {
        cout << "Robot is not on the table" << endl;
    }
    else
    {
        cout << "Robot is at x = " << m_xpos << ", y = " << m_ypos
             << ", facing " << directionAsString(m_direction) << endl;
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
    // Aren't we generous :-)
    return ( str == "N" || str == "NORTH" ) ? North :
           ( str == "W" || str == "WEST" )  ? West :
           ( str == "S" || str == "SOUTH" ) ? South :
           ( str == "E" || str == "EAST" )  ? East :
                                              Invalid;
}
