function [] = plot_gesture_dbl_swipe(port_num)

%*
% * % * Copyright (C) {2018} Texas Instruments Incorporated - http://www.ti.com/ 
% * ALL RIGHTS RESERVED 
% * 
% * Reads the data output and plots radar data and gesture detection flag
% * 
% */
global datavec;
global framecou;
global GESTURE_PKT_SIZE_BYTES;
global NUM_PKTS_TO_COLLECT;
global data_log;
global truedetectioncount;
global threshdata;
global flagvals;
global allbytes;
global finalP;
truedetectioncount = 0;
NUM_PKTS_TO_COLLECT =8;
GESTURE_PKT_SIZE_BYTES=48;
data_log=[];
datavec = [];
allbytes = [];
framecou = 0;
finalP = [];
threshdata = [];
flagvals = [];

% Data stream variables
BUFFER_SIZE=100;
x_sample = 1:BUFFER_SIZE;
numdetections=zeros(1,BUFFER_SIZE);
dopplerave=zeros(1,BUFFER_SIZE);
rangeave=zeros(1,BUFFER_SIZE);
angleval=zeros(1,BUFFER_SIZE);
rangeidx=zeros(1,BUFFER_SIZE);
magsum=zeros(1,BUFFER_SIZE);
doppler_disp = zeros(1,BUFFER_SIZE);
range_disp = zeros(1,BUFFER_SIZE);
prediction_result = zeros(1,BUFFER_SIZE);

vel_resolution = 0.0380479;
range_resolution = 0.046875;

% vars for classfication from target
Takeprediction_fromtarget = 1;

% configuration
sphandle = configureSport(port_num);
ctr=0;

% plot options
plot_datastream = 1;


%% Define main figure


    h = figure(1);

    set(h,'Name','Gesture Demo', 'color',[0.975 0.975 0.975], 'MenuBar','none','ToolBar','none','units','normalized','OuterPosition',[0.5 0 0.5 1])
    %suptitle('Gesture: Flick wrist LEFT-RIGHT-LEFT')
    t = annotation('textbox', 'String','Gesture: Double Swipe from L to R to L', 'units','normalized', 'position', [0.05 0.9 0.85 0.1]);
    t.LineStyle = 'none';
    t.FontUnits = 'normalized';
    t.FontSize = 0.025;
    
    note1 = annotation('textbox', 'String','GREEN = Gesture Detected', 'units','normalized', 'position', [0.05 0.285 0.85 0.1]);
    note1.LineStyle = 'none';
    note1.FontUnits = 'normalized';
    note1.FontSize = 0.025;
    
    %instruction image
    subplot(3,2,1), imshow('door_ctrl_pic.png');

 if (plot_datastream)
    %init plots
    s2 = subplot(3,2,2);  
    p2 = plot(x_sample, numdetections); hold on;
    p2_marker = plot([-1 -1], [0 0], 'g--', 'LineWidth', 3); hold off;
    title('Active Bins in Range-Doppler'); xlim([0 100]); ylim([0 200]); 
    

    s3 = subplot(3,2,3); grid on; axis manual;
    p3 = plot(dopplerave*vel_resolution); hold on;
    p3_marker = plot([-1 -1], [0 0], 'g--','LineWidth', 3); hold off;
    title('Weighted Doppler Signature'); xlim([0 100]); ylim([-2 2]); 


    s4 = subplot(3,2,4); grid on; axis manual;
    p4 = plot(asind(angleval/32)); %ylabel('in degrees');
    hold on;
    p4_marker = plot([-1 -1], [0 0], 'g--','LineWidth', 3); hold off;
    title('Angle'); xlim([0 100]); ylim([-70 70]); 
 end
   



%% main 
while (ishghandle(h))
    %disp(get(sphandle,'BytesAvailable'));
    
    if ~isempty(datavec)
        
        if (datavec(1)==11) %check magic word
            ctr=ctr+1;
            %Reshape packet for data extraction
            datavec1=reshape(datavec,GESTURE_PKT_SIZE_BYTES/4,NUM_PKTS_TO_COLLECT)	;
            

            %Extract radar data
            if (plot_datastream)
                numdetections=[numdetections(1+NUM_PKTS_TO_COLLECT:end) datavec1(2,:) ]; 
                dopplerave=[dopplerave(1+NUM_PKTS_TO_COLLECT:end) datavec1(3,:) ]; 
                rangeave=[rangeave(1+NUM_PKTS_TO_COLLECT:end) datavec1(4,:) ]; 
                magsum=[magsum(1+NUM_PKTS_TO_COLLECT:end) datavec1(5,:) ]; 
                angleval=[angleval(1+NUM_PKTS_TO_COLLECT:end) datavec1(6,:) ]; 
                rangeidx=[rangeidx(1+NUM_PKTS_TO_COLLECT:end) datavec1(7,:) ]; 
                range_disp=[range_disp(1+NUM_PKTS_TO_COLLECT:end) datavec1(10,:)];
                doppler_disp=[doppler_disp(1+NUM_PKTS_TO_COLLECT:end) datavec1(11,:)];
            end
            
            %Extract on board classifier result 
            if Takeprediction_fromtarget
                prediction_result = [prediction_result(1+NUM_PKTS_TO_COLLECT:end) datavec1(12,:)];
            end

          %% Plot figure 
            if(plot_datastream)
                set(p2, 'YData', numdetections)
                set(p3, 'YData', dopplerave*vel_resolution)
                set(p4, 'YData', asind(angleval/32))
            end

            % Plot classification result from IWR1443
            if Takeprediction_fromtarget
                subplot(3,1,3);
                plot_pred_marker(prediction_result, [p2_marker, p3_marker, p4_marker])
                if(sum(prediction_result(end-7:end))>0)
                    set(bar(1),'facecolor','g');                
                else
                    set(bar(0),'facecolor','g');	
                end
                title('Handcrafted Classifier on IWR1443');
                ylim([0,1]);  set(gca, 'ytick',[],'yticklabel',[],'xtick',[],'xticklabel',[]);
            end

 
        %Clear data    
        datavec=[];
        
        else
            disp('missing magic word');
            %%uiwait(msgbox(sprintf('Lost sync of serial port data(%d)',get(sphandle,'BytesAvailable')),'Error','modal'));
            numextra = get(sphandle,'BytesAvailable');
            if numextra > 0
                fread(sphandle,numextra,'uint8');
            end
            datavec = [];
            title(sprintf('Serial port sync loss!  Frame number = %d',framecou));
            drawnow;
        end %datavec(1) == 11 / checking magic word       
    end %~isempty(datavec)   
    pause(0.02);
    
end %while(1)
return

function [] = plotImage(obj, event)
    global GESTURE_PKT_SIZE_BYTES
    global NUM_PKTS_TO_COLLECT
    global datavec;
    global framecou;

    datavec = fread(obj, GESTURE_PKT_SIZE_BYTES*NUM_PKTS_TO_COLLECT/4, 'float');
    framecou = framecou + 1;
return

function [] = dispError()
    disp('error!');
return

function [sphandle] = configureSport(port_num)
    global GESTURE_PKT_SIZE_BYTES
    global NUM_PKTS_TO_COLLECT;
    if ~isempty(instrfind('Type','serial'))
        disp('Serial port(s) already open. Re-initializing...');
        delete(instrfind('Type','serial'));  % delete open serial ports.
    end
    sphandle = serial(['COM' num2str(port_num)],'BaudRate',921600);
    set(sphandle,'InputBufferSize',GESTURE_PKT_SIZE_BYTES*NUM_PKTS_TO_COLLECT);
    set(sphandle,'Timeout',10);
    set(sphandle,'ErrorFcn',@dispError);
    set(sphandle,'BytesAvailableFcnMode','byte');
    set(sphandle,'BytesAvailableFcnCount',GESTURE_PKT_SIZE_BYTES*NUM_PKTS_TO_COLLECT);
    set(sphandle,'BytesAvailableFcn',@plotImage);
    fopen(sphandle);
return

function [] = exitDemo(sphandle)
    fclose(sphandle);
    delete(sphandle);
    clear sphandle
return