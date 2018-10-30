function [ports] = get_com_ports()
%*
% * % * Copyright (C) {2018} Texas Instruments Incorporated - http://www.ti.com/ 
% * ALL RIGHTS RESERVED 
% * 
% * Reads the data output and plots radar data and gesture detection flag
% * 
% */    
    
    command = 'wmic path win32_pnpentity get caption /format:list | find "COM"';
    [status, cmdout] = system (command)
    UART_COM = regexp(cmdout, 'UART\s+\(COM[0-9]+', 'match')
    UART_COM = (regexp(UART_COM, '[0-9]+', 'match'))
    DATA_COM = regexp(cmdout, 'Data\s+Port\s+\(COM[0-9]+', 'match');
    DATA_COM = (regexp(DATA_COM, '[0-9]+', 'match'));
    
    n = length(UART_COM);
    
    CLI_PORT = zeros(n,1);
    S_PORT = zeros(n,1)
    
    for i=1:n
        CLI_PORT(i,1) = str2num(cell2mat(UART_COM{1,i}));
        S_PORT(i,1) = str2num(cell2mat(DATA_COM{1,i}));
    end
    
    CLI_PORT = sort(CLI_PORT);
    S_PORT = sort(S_PORT);
    ports = [CLI_PORT, S_PORT];
end