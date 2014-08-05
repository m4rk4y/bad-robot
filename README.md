bad-robot
=========

Poor solution to "toy robot" coding exercise

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
