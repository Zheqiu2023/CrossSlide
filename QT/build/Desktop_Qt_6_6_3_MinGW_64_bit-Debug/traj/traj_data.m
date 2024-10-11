clear,clc

LEAD = 5;
RE35_REDUCTION_RATIO = 35;
ENCODER_LINES_NUM = 2000;

%% 处理指令数据
send_data = readmatrix('../20241011_002316_send.csv');

time_vec = send_data(:, 1);
base_time = time_vec(1);
adjusted_time_vec = (time_vec - base_time) / 1000;

target_pos = send_data(:, 2:end);

%% 处理电机反馈数据
recv_data = readmatrix('../20241011_002305_recv.csv');

real_time = (recv_data(:, 1) - base_time) / 1000;
x_pos = -recv_data(:, 3) * LEAD / (RE35_REDUCTION_RATIO * ENCODER_LINES_NUM);
y_pos = -recv_data(:, 5) * LEAD / (RE35_REDUCTION_RATIO * ENCODER_LINES_NUM);
z_pos = -recv_data(:, 7) * LEAD / (RE35_REDUCTION_RATIO * ENCODER_LINES_NUM);

%% 计算误差
for i = 1:length(time_vec)
    [~,index] = min(abs(real_time - adjusted_time_vec(i)));
    x_error(i) = x_pos(index) - target_pos(i, 1);
    y_error(i) = y_pos(index) - target_pos(i, 2);
    z_error(i) = z_pos(index) - target_pos(i, 3);
end

%% 绘图
figure;

subplot(3, 1, 1);   
plot(adjusted_time_vec, target_pos(:, 1), "Color", 'r', 'LineWidth', 1); hold on;
plot(real_time, x_pos, "Color", 'b', 'LineWidth', 1); hold on;
plot(adjusted_time_vec, x_error, "Color", 'g', 'LineWidth', 1); 
title('X');
xlabel('时间 (s)');
ylabel('位置 (mm)');
legend({'期望位置', '实际位置', '位置误差'});
xlim([0 30]);
grid on;

subplot(3, 1, 2);   
plot(adjusted_time_vec, target_pos(:, 2), "Color", 'r', 'LineWidth', 1); hold on;
plot(real_time, y_pos, "Color", 'b', 'LineWidth', 1); hold on;
plot(adjusted_time_vec, y_error, "Color", 'g', 'LineWidth', 1); 
title('Y');
xlabel('时间 (s)');
ylabel('位置 (mm)');
legend({'期望位置', '实际位置', '位置误差'});
xlim([0 30]);
grid on;

subplot(3, 1, 3);   
plot(adjusted_time_vec, target_pos(:, 3), "Color", 'r', 'LineWidth', 1); hold on;
plot(real_time, z_pos, "Color", 'b', 'LineWidth', 1); hold on;
plot(adjusted_time_vec, z_error, "Color", 'g', 'LineWidth', 1); 
title('Z');
xlabel('时间 (s)');
ylabel('位置 (mm)');
legend({'期望位置', '实际位置', '位置误差'});
xlim([0 30]);
grid on;

