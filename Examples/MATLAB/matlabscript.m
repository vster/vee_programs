fig=findobj('Type','Figure','Tag',tagname);
if isempty(fig)
  logo
  fig=gcf
  set(fig,'Tag',tagname,'Color',[0 0 0],'Menubar','none')
else
  figure(fig)
end
 
L=40*membrane(1,m);
edge=1:(2*m+1);
s=findobj(fig,'Type','surface'); 
set(s,'XData',edge,'YData',edge,'ZData',L);