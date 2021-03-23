#!/bin/bash

OPTIONS=":?hn"


HEADERS=false
NORMALIZE=false


usage() {
    echo "$0: usage: $0 [-hn?] pathfile" >&2
    echo "     -h add headers to output" >&2
    echo "     -n normalize output based on first record" >&2
	echo "     -? usage" >&2
}


while getopts ${OPTIONS} arg
do
    case ${arg} in
    h)
        HEADERS=true
        ;;
    n)
        NORMALIZE=true
        ;;
    ?)
        usage
        exit 0
        ;;
    ?)
        echo "$0: error: invalid option: -${OPTARG}"
        exit 1
        ;;
    esac
done


# move past the options
shift $((OPTIND-1))


if [ ! "$1" = "" ]
then
    DATA_FILE="$1"
else
    echo "$0: error: no input pathfiel specified"
    usage
    exit 1
fi

if [ ! -f $DATA_FILE ]
then
    echo "$0: error: \"$DATA_FILE\" does not exist"
    exit 1
fi


# build each data array using a distinct pass of jq - this means you don't
# have to parse data values apart and you get each field in a very clean
# and distinct manner
#
# let the magic do the work for you...

TIME_ARRAY=(`jq '.[] .time' <$DATA_FILE`)
VELOCITY_ARRAY=(`jq '.[] .velocity' <$DATA_FILE`)
ACCEL_ARRAY=(`jq '.[] .acceleration' <$DATA_FILE`)
TRANS_X_ARRAY=(`jq '.[] .pose.translation.x' <$DATA_FILE`)
TRANS_Y_ARRAY=(`jq '.[] .pose.translation.y' <$DATA_FILE`)
ROTATION_ARRAY=(`jq '.[] .pose.rotation.radians' <$DATA_FILE`)
CURVATURE_ARRAY=(`jq '.[] .curvature' <$DATA_FILE`)

COUNT=`jq '.[] .time' <$DATA_FILE | wc -l`


# normalize the output based on the first trajectory record
#
# this is exactly what you would do in the robot code using:
#
#              m_drive.ResetOdometry(exampleTrajectory.InitialPose());
#
# (or something like that)

if [ $NORMALIZE = "true" ]
then
    NORMALIZE_X=${TRANS_X_ARRAY[0]}
    NORMALIZE_Y=${TRANS_Y_ARRAY[0]}

    echo "Normalizing using $NORMALIZE_X, $NORMALIZE_Y as initial pose..." >&2

    INDEX=0

    while [ $INDEX -lt $COUNT ]
    do
        PERL_X_CMD="print ${TRANS_X_ARRAY[$INDEX]} - $NORMALIZE_X"
        PERL_Y_CMD="print ${TRANS_Y_ARRAY[$INDEX]} - $NORMALIZE_Y"

        # need to externalize the normalization calculations as bash
        # doesn't support floating point calculations

        TRANS_X_ARRAY[$INDEX]=`perl -e "${PERL_X_CMD}"`
        TRANS_Y_ARRAY[$INDEX]=`perl -e "${PERL_Y_CMD}"`

        INDEX=$(($INDEX + 1))
    done
fi


# generate the output

if [ $HEADERS = "true" ]
then
    echo "Time, Velocity, Acceleration, Pose X, Pose Y, Pose Rotation, Curvature"
fi


INDEX=0

while [ $INDEX -lt $COUNT ]
do
    echo "${TIME_ARRAY[$INDEX]}, ${VELOCITY_ARRAY[$INDEX]}, ${ACCEL_ARRAY[$INDEX]}, ${TRANS_X_ARRAY[$INDEX]}, ${TRANS_Y_ARRAY[$INDEX]}, ${ROTATION_ARRAY[$INDEX]}, ${CURVATURE_ARRAY[$INDEX]}"

    INDEX=$(($INDEX + 1))
done

