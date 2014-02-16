function analyse(str, A, B)
	meanA = mean(A(:));
	meanB = mean(B(:));
	
	DA = im2double(A);
	DB = im2double(B);

	varA = var(DA(:));
	varB = var(DB(:));

	kurA = kurtosis(DA(:));
	kurB = kurtosis(DB(:));

	printf("%s\n", str);
	printf("A: mean=%f, variance=%f, kurtosis=%f\n", meanA, varA, kurA);
	printf("B: mean=%f, variance=%f, kurtosis=%f\n", meanB, varB, kurB);
end;
