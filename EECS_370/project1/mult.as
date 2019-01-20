		lw 0 0 zero load 0 into reg0
		lw 0 2 mcand read mcand into reg2
		lw 0 3 mplier read mplier into reg3
		lw 0 6 one load into reg 6 which is forloop tracker
		lw 0 7 end load 65536 into reg 7 which is forloop end condition
		nor 3 3 5 store mplier nor mplier into reg 5
loop	beq 6 7 done
		nor 6 6 4 nor mask with itself
		nor 4 5 4 nor nord mask with nor mplier reg3 has mask & mplier
		beq 0 4 skip if 0 skip add
		add 1 2 1 add the mcand to the result 
skip	add 2 2 2 lsl mcand by 1
		add 6 6 6 forloop increment by 1
		beq 0 0 loop
done halt
zero .fill 0
one .fill 1 
end .fill 65536
mcand .fill 32766
mplier .fill 10383