%*
% * % * Copyright (C) {2018} Texas Instruments Incorporated - http://www.ti.com/ 
% * ALL RIGHTS RESERVED 
% * 
% * Main for loading config and running swipe demo
% * 
% */

clear, clc, close all
delete(instrfind)

%% get COM ports for EVM
ports = get_com_ports();
portStr = {};
for i =1:length(ports(:,1))
    portStr{end+1} = ['COM ' num2str(ports(i,1)) ' and ' num2str(ports(i,2))];
end
       
       
[s,ok] = listdlg('PromptString','Select IWR14XX EVM ports:',...
               'SelectionMode','single',...
               'ListString', portStr);
if(ok)           
    %% load config
    set(0,'DefaultFigureVisible','off')
    mmw_demo('xwr14xx', ports(s,2) , 1, 0.5, ports(s,1), 'profile_2d_gesture_128.cfg', 1)
    set(0,'DefaultFigureVisible', 'on')
    close all
    %% run demo
    plot_gesture_dbl_swipe(ports(s,2))
else
    error('COM port not selected. Exiting')
end