#!/bin/sh
#
# Unreal Tournament 2003 startup script
#

# Function to find the real directory a program resides in.
# Feb. 17, 2000 - Sam Lantinga, Loki Entertainment Software
FindPath()
{
    fullpath="`echo $1 | grep /`"
    if [ "$fullpath" = "" ]; then
        oIFS="$IFS"
        IFS=:
        for path in $PATH
        do if [ -x "$path/$1" ]; then
               if [ "$path" = "" ]; then
                   path="."
               fi
               fullpath="$path/$1"
               break
           fi
        done
        IFS="$oIFS"
    fi
    if [ "$fullpath" = "" ]; then
        fullpath="$1"
    fi

    # Is the sed/ls magic portable?
    if [ -L "$fullpath" ]; then
        #fullpath="`ls -l "$fullpath" | awk '{print $11}'`"
        fullpath=`ls -l "$fullpath" |sed -e 's/.* -> //' |sed -e 's/\*//'`
    fi
    dirname $fullpath
}

# Set the home if not already set.
if [ "${UT2003_DATA_PATH}" = "" ]; then
    UT2003_DATA_PATH="`FindPath $0`/System"
fi

LD_LIBRARY_PATH=.:${UT2003_DATA_PATH}:${LD_LIBRARY_PATH}

export LD_LIBRARY_PATH

# Let's boogie!
if [ -x "${UT2003_DATA_PATH}/ut2003-bin" ]
then
	cd "${UT2003_DATA_PATH}/"
	exec "./ut2003-bin" $*
fi
echo "Couldn't run Unreal Tournament 2003 (ut2003-bin). Is UT2003_DATA_PATH set?"
exit 1

# end of ut2003 ...

