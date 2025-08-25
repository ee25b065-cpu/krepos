#EE25B065,EE25B066,EE25B075

EXECUTABLE="./EE25B065_trig"
THETA=45
START_N=1
END_N=20
echo " N   Error_sine     Error_cosine" 
for ((N=$START_N; N<=$END_N; N++)); do
    output=$($EXECUTABLE $N $THETA)
    echo " $output"
done
