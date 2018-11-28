close all
pause('on')
n=2^15;
x_data = x_points{1};
y_data = y_points{1};
z_data = z_points{1};
figure,hold on, grid on
h = scatter3(x_data,y_data,z_data,'b','filled','LineWidth',0.1);
xlabel('x-axis'),ylabel('y-axis'),zlabel('z-axis');
xlim([-2 2]), ylim([-0.1 10]), zlim([-2 2]);
h.XDataSource = 'x_data';
h.YDataSource = 'y_data';
h.ZDataSource = 'z_data';
for i=2:length(x_points)
    n=1;
    x_data = x_points{i};
    y_data = y_points{i};
    z_data = z_points{i};
%     pause(1)
%     refreshdata
scatter3(x_data,y_data,z_data,'b','filled','LineWidth',0.1);
end
hold off;