clear
fid = fopen('data_13148





.dat','r');
adcData = fscanf(fid,'%d');
d = dec2hex(adcData,2);
d = d(:,4:5);
%%
n_headers = 0;
n=0;
pckt_length = [];
while (n<length(adcData))
    pckt_length = [pckt_length,hex2dec(reshape(flip(d(n+13:n+16,:))',1,[]))];
    n = n+hex2dec(reshape(flip(d(n+13:n+16,:))',1,[]));
    n_headers = n_headers+1;
end
%%
ind=0;
for i=1:n_headers
    
    %% header = d(1:36,:);
    magic_word = [flip(d(ind+1:ind+2,:));flip(d(ind+3:ind+4,:));flip(d(ind+5:ind+6,:));flip(d(ind+7:ind+8,:))];
    for j=1:8
        if(strcmp(magic_word(j,:),['0',num2str(j)]))
            continue;
        else
            error('Unrecognised file: Wrong Magic word')
        end
    end
    version_sdk = hex2dec(d(ind+9:ind+12,:));
    if(version_sdk ~= [2;0;1;1])
        err('Wrong SDK version')
    end
    ttl_pckt_length = hex2dec(reshape(flip(d(ind+13:ind+16,:))',1,[]));
    platform  = reshape(flip(d(ind+17:ind+20,:))',1,[]);
    if (~strcmp(platform,'000A1443'))
        err('Wrong platform')
    end
    frame_number = hex2dec(reshape(flip(d(ind+21:ind+24,:))',1,[]));
    n_CPU_cycles = hex2dec(reshape(flip(d(ind+25:ind+28,:))',1,[]));
    n_detected_obj = hex2dec(reshape(flip(d(ind+29:ind+32,:))',1,[]));
    n_TLVs = hex2dec(reshape(flip(d(ind+33:ind+36,:))',1,[]));
    count = ind+36;
    TLV_tag = zeros(n_TLVs,1);
    TLV_length = zeros(n_TLVs,1);
    for j=1:n_TLVs
        TLV_tag(j) = hex2dec(reshape(flip(d(count+1:count+4,:))',1,[]));
        TLV_length(j) = hex2dec(reshape(flip(d(count+5:count+8,:))',1,[]));
        
        if(TLV_tag(j)==1)
            n_objects(i) = hex2dec(reshape(flip(d(count+9:count+10,:))',1,[]));
            q_format(i) = hex2dec(reshape(flip(d(count+11:count+12,:))',1,[]));
            nor = 2^(q_format(i));
            count = count+12;
            rande_idx{i} = zeros(n_objects(i),1);
            doppler_idx{i} = zeros(n_objects(i),1);
            max_values{i} = zeros(n_objects(i),1);
            x_points{i} = zeros(n_objects(i),1);
            y_points{i} = zeros(n_objects(i),1);
            z_points{i} = zeros(n_objects(i),1);
            for k=1:n_objects(i)
                rande_idx{i}(k) = hex2dec(reshape(flip(d(count+1:count+2,:))',1,[]));
                doppler_idx{i}(k) = hex2dec(reshape(flip(d(count+3:count+4,:))',1,[]));
                max_values{i}(k) = hex2dec(reshape(flip(d(count+5:count+6,:))',1,[]));
                x_points{i}(k) = double(typecast(uint16(hex2dec(reshape(flip(d(count+7:count+8,:))',1,[]))),'int16'))./nor;
                y_points{i}(k) = double(typecast(uint16(hex2dec(reshape(flip(d(count+9:count+10,:))',1,[]))),'int16'))./nor;
                z_points{i}(k) = double(typecast(uint16(hex2dec(reshape(flip(d(count+11:count+12,:))',1,[]))),'int16'))./nor;
                count = count+12;
            end
            break
        else
            continue
        end
    end
    ind = ind + pckt_length(i);
end