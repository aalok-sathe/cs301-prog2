lw $7, 0($0)
sub $1, $7, $2
addi $4, $1, 3
j label
lw $4, 12($7)
lw $3, 4($2)
slti $1, $4, 6
add $3, $1, $4
srl $1, $2, 2
sra $1, $1, 3
