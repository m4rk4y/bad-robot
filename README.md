bad-robot
=========

Poor solution to "toy robot" coding exercise

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
