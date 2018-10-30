function [ ] = plot_pred_marker(pred_result, p_h_marker)
%*
% * % * Copyright (C) {2018} Texas Instruments Incorporated - http://www.ti.com/ 
% * ALL RIGHTS RESERVED 
% * 
% * Plots a green line marker to indicate time of gesture recognition
% * 
% */
indx_detection = find(pred_result);
recent_pred = max(indx_detection);
if(isempty(recent_pred))
    recent_pred = -10;
end

for i = 1 : length(p_h_marker)
    YLim = p_h_marker(i).Parent.YLim;
    set(p_h_marker(i), 'XData', [recent_pred recent_pred], 'YData', YLim)


      
end
end

