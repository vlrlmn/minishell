cc -Wall -Wextra -Werror get_token.c ../Libft/libft.a -fsanitize=address
./a.out "ls -a"
./a.out "ls -a < infile"
./a.out "< infile ls -a"
./a.out "< infile ls -a << heredoc"
./a.out "< infile << heredoc ls -a "
./a.out "ls -a < infile << heredoc"
./a.out "<< heredoc ls -a < infile"
./a.out "< infile ls -a < infile < infile"
./a.out " > outfile < infile ls -a < infile > outfile < infile"
./a.out "<< H"
rm ./a.out