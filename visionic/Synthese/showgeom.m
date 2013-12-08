function varargout = ShowGeom(varargin)
% SHOWGEOM Application M-file for ShowGeom.fig
%    FIG = SHOWGEOM launch ShowGeom GUI.
%    SHOWGEOM('callback_name', ...) invoke the named callback.
%
% Le programme ShowGeom permet de vérifier interactivement la géométrie
% épipolaire d'un couple d'image stéréo. Appelée sans paramètres, la
% fonction ShowGeom utilise les variables img1, cmap1, img2, cmap2 et F
% définies dans l'espace de travail. Il est possible de spécifier ces
% paramètres en les passant directement à la fonction (méthode conseillée) :
%   ShowGeom(img1, cmap1, img2, cmap2, F);
%
% Définition des paramètres :
%  img1 : image à afficher à gauche
%  cmap1 : table des couleurs pour l'image 1 si img1 est une image indexée,
%          sinon passer [].
%  img2 : image à afficher à droite
%  cmap2 : table des couleurs pour l'image 2 si img2 est une image indexée,
%          sinon passer [].
%  F : Matrice fondamentale
%  

% Last Modified by GUIDE v2.0 01-Nov-2003 18:05:49

if nargin == 0  % LAUNCH GUI

    warning('L''utilisation de cette fonction sans paramètre est déconseillé !');
    global img1 img2 cmap1 cmap2 F
    img1 = evalin('base', 'img1', '[]');
    cmap1 = evalin('base', 'cmap1', '[]');
    img2 = evalin('base', 'img2', '[]');
    cmap2 = evalin('base', 'cmap2', '[]');
    F = evalin('base', 'F', '[]');
    if isempty(img1) | isempty(img2) | isempty(F)
        return
    end

	fig = openfig(mfilename,'reuse');

	% Use system color scheme for figure:
	set(fig,'Color',get(0,'defaultUicontrolBackgroundColor'));

	% Generate a structure of handles to pass to callbacks, and store it. 
	handles = guihandles(fig);
	guidata(fig, handles);

	if nargout > 0
		varargout{1} = fig;
	end

    figure1_ResizeFcn(0, 0, handles, 0);

    figure(fig);
    axes(handles.img1);
    h = image(img1);
    set(h, 'HandleVisibility', 'callback');
    %set(h, 'EraseMode', 'none');
    colormap(cmap1);
    set(handles.img1, 'DataAspectRatio', [1 1 1]);
    axis off;

    axes(handles.img2);
    h = image(img2);
    set(h, 'HandleVisibility', 'callback');
    %set(h, 'EraseMode', 'none');
    colormap(cmap2);
    set(handles.img2, 'DataAspectRatio', [1 1 1]);
    axis off;

elseif ischar(varargin{1}) % INVOKE NAMED SUBFUNCTION OR CALLBACK

	try
		if (nargout)
			[varargout{1:nargout}] = feval(varargin{:}); % FEVAL switchyard
		else
			feval(varargin{:}); % FEVAL switchyard
		end
	catch
		disp(lasterr);
	end

else
    if nargin ~= 5
        error('Nombre de paramètres incorrect');
    end
    global img1 img2 cmap1 cmap2 F
    img1 = varargin{1};
    cmap1 = varargin{2};
    img2 = varargin{3};
    cmap2 = varargin{4};
    F = varargin{5};
    if isempty(img1) | isempty(img2) | isempty(F)
        return
    end

	fig = openfig(mfilename,'reuse');

	% Use system color scheme for figure:
	set(fig,'Color',get(0,'defaultUicontrolBackgroundColor'));

	% Generate a structure of handles to pass to callbacks, and store it. 
	handles = guihandles(fig);
	guidata(fig, handles);

	if nargout > 0
		varargout{1} = fig;
	end

    figure1_ResizeFcn(0, 0, handles, 0);

    figure(fig);
    axes(handles.img1);
    h = image(img1);
    set(h, 'HandleVisibility', 'callback');
    %set(h, 'EraseMode', 'none');
    colormap(cmap1);
    set(handles.img1, 'DataAspectRatio', [1 1 1]);
    axis off;

    axes(handles.img2);
    h = image(img2);
    set(h, 'HandleVisibility', 'callback');
    %set(h, 'EraseMode', 'none');
    colormap(cmap2);
    set(handles.img2, 'DataAspectRatio', [1 1 1]);
    axis off;
    
end


%| ABOUT CALLBACKS:
%| GUIDE automatically appends subfunction prototypes to this file, and 
%| sets objects' callback properties to call them through the FEVAL 
%| switchyard above. This comment describes that mechanism.
%|
%| Each callback subfunction declaration has the following form:
%| <SUBFUNCTION_NAME>(H, EVENTDATA, HANDLES, VARARGIN)
%|
%| The subfunction name is composed using the object's Tag and the 
%| callback type separated by '_', e.g. 'slider2_Callback',
%| 'figure1_CloseRequestFcn', 'axis1_ButtondownFcn'.
%|
%| H is the callback object's handle (obtained using GCBO).
%|
%| EVENTDATA is empty, but reserved for future use.
%|
%| HANDLES is a structure containing handles of components in GUI using
%| tags as fieldnames, e.g. handles.figure1, handles.slider2. This
%| structure is created at GUI startup using GUIHANDLES and stored in
%| the figure's application data using GUIDATA. A copy of the structure
%| is passed to each callback.  You can store additional information in
%| this structure at GUI startup, and you can change the structure
%| during callbacks.  Call guidata(h, handles) after changing your
%| copy to replace the stored original so that subsequent callbacks see
%| the updates. Type "help guihandles" and "help guidata" for more
%| information.
%|
%| VARARGIN contains any extra arguments you have passed to the
%| callback. Specify the extra arguments by editing the callback
%| property in the inspector. By default, GUIDE sets the property to:
%| <MFILENAME>('<SUBFUNCTION_NAME>', gcbo, [], guidata(gcbo))
%| Add any extra arguments after the last argument, before the final
%| closing parenthesis.



% --------------------------------------------------------------------
function varargout = figure1_WindowButtonMotionFcn(h, eventdata, handles, varargin)
global F

MaxSize = get(handles.Form, 'Position');
Pos = get(handles.Form, 'CurrentPoint');
if Pos(1)<MaxSize(3)/2
    %la souris est sur l'image 1
    
    %récupération des coordonnées du points courant
    Pos = get(handles.img1, 'CurrentPoint');
    
    %calcul de l'équation de la droite épipolaire dans l'image 2 :
    u = Pos(1,1);
    v = Pos(1,2);
    l = F*[u, v, 1]';
    
    axes(handles.img2);
    a = axis;
    umax = a(2);
    umin = a(1);
    vmax = a(4);
    vmin = a(3);
    %cla;
    if l(2)==0
        p1 = [-l(3)/l(1), vmin];
        p2 = [-l(3)/l(1), vmax];
    else
        p1 = [umin, (-l(3)-l(1)*umin)/l(2)];
        p2 = [umax, (-l(3)-l(1)*umax)/l(2)];
    end
    hLine = findobj(gca,'Type','line');
    if size(hLine,1) == 0
        line([p1(1), p2(1)], [p1(2), p2(2)], 'Color', 'r');
    else
        set(hLine, 'XData', [p1(1), p2(1)]);
        set(hLine, 'YData', [p1(2), p2(2)]);
    end
    
    %calcul de l'équation de la droite épipolaire dans l'image 1 :
    l = F'*[p1(1), p1(2), 1]';
    
    axes(handles.img1);
    a = axis;
    umax = a(2);
    umin = a(1);
    vmax = a(4);
    vmin = a(3);
    %cla;
    if l(2)==0
        p1 = [-l(3)/l(1), vmin];
        p2 = [-l(3)/l(1), vmax];
    else
        p1 = [umin, (-l(3)-l(1)*umin)/l(2)];
        p2 = [umax, (-l(3)-l(1)*umax)/l(2)];
    end
    hLine = findobj(gca,'Type','line');
    if size(hLine,1) == 0
        line([p1(1), p2(1)], [p1(2), p2(2)], 'Color', 'r');
    else
        set(hLine, 'XData', [p1(1), p2(1)]);
        set(hLine, 'YData', [p1(2), p2(2)]);
    end
    

else
    %la souris est sur l'image 2
    
    %récupération des coordonnées du points courant
    Pos = get(handles.img2, 'CurrentPoint');
    
    %calcul de l'équation de la droite épipolaire dans l'image 1 :
    u = Pos(1,1);
    v = Pos(1,2);
    l = F'*[u, v, 1]';
    
    axes(handles.img1);
    a = axis;
    umax = a(2);
    umin = a(1);
    vmax = a(4);
    vmin = a(3);
    %cla;
    if l(2)==0
        p1 = [-l(3)/l(1), vmin];
        p2 = [-l(3)/l(1), vmax];
    else
        p1 = [umin, (-l(3)-l(1)*umin)/l(2)];
        p2 = [umax, (-l(3)-l(1)*umax)/l(2)];
    end
    hLine = findobj(gca,'Type','line');
    if size(hLine,1) == 0
        line([p1(1), p2(1)], [p1(2), p2(2)], 'Color', 'r');
    else
        set(hLine, 'XData', [p1(1), p2(1)]);
        set(hLine, 'YData', [p1(2), p2(2)]);
    end
    
    %calcul de l'équation de la droite épipolaire dans l'image 2 :
    l = F*[p1(1), p1(2), 1]';
    
    axes(handles.img2);
    a = axis;
    umax = a(2);
    umin = a(1);
    vmax = a(4);
    vmin = a(3);
    %cla;
    if l(2)==0
        p1 = [-l(3)/l(1), vmin];
        p2 = [-l(3)/l(1), vmax];
    else
        p1 = [umin, (-l(3)-l(1)*umin)/l(2)];
        p2 = [umax, (-l(3)-l(1)*umax)/l(2)];
    end
    hLine = findobj(gca,'Type','line');
    if size(hLine,1) == 0
        line([p1(1), p2(1)], [p1(2), p2(2)], 'Color', 'r');
    else
        set(hLine, 'XData', [p1(1), p2(1)]);
        set(hLine, 'YData', [p1(2), p2(2)]);
    end
    

end



% --------------------------------------------------------------------
function varargout = figure1_ResizeFcn(h, eventdata, handles, varargin)
MaxSize = get(handles.Form, 'Position');
set(handles.img1, 'Position', [0, 0, MaxSize(3)/2, MaxSize(4)]);
set(handles.img2, 'Position', [MaxSize(3)/2, 0, MaxSize(3)/2, MaxSize(4)]);

