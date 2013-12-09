function MethodName = GetMethodName(method)
%  GetMethodName(method)
%
% Description :
%  Retourne le nom de la m�thode associ� � un entier.
%
% Param�tres :
%  method : index de la m�thode. Si method = -1, la fonction retourne la
%           liste compl�te des m�thodes
%
% Valeurs retourn�es :
%  MethodName : Nom de la m�thode.
%


switch method
    case -1
        MethodName = ' # : Nom de la m�thode';
        for i=1:10
            MethodName = strcat(MethodName,...
                                sprintf('\n %d : %s', i, GetMethodName(i)));
        end
    case 0
        MethodName = 'Nothing';
    case 1
        MethodName = '(robust) MAPSAC 7 point';
    case 2
        MethodName = '(linear) Orthogonal least squares';
    case 3
        MethodName = '(linear) Bookstein : invariant total least squares';
    case 4
        MethodName = '(iterative) Bookstein-Sampson : re-weighted invariant total least squares';
    case 5
        MethodName = 'indisponible';%'Non-linear : MATLAB fmincon';
    case 6
        MethodName = 'indisponible';%'Linear followed by non-linear';
    case 7
        MethodName = '(linear) 8-point';
    case 8
        MethodName = '(robust-linear) MAPSAC 7-point followed by 8-point';
    case 9
        MethodName = 'indisponible';%'MAPSAC 8-point';
    case 10
        MethodName = 'indisponible';%'MAPSAC 8-point followed by 8-point';
    otherwise
        MethodName = 'Unknown method';
end
