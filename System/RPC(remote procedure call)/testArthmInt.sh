#!/bin/sh
arthm=./bin/./client

for i in 10 20 30 40 50 60 70 80 90 100
do
    $arthm addtion int $i int $i
    $arthm soustraction int $i int 13
    $arthm multiplication int $i int 3
    $arthm division int $i int 2
done

for j in ab bc cd de ef
do
    $arthm addition string $j string zzz
    $arthm addition string $j string 123
done

$arthm ackerman int 20 int 20

