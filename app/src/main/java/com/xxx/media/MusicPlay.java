package com.xxx.media;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class MusicPlay {
    private AudioTrack audioTrack;

    public void createTrack(int sampleRateInHz, int nb_channals) {
        int channaleConfig;//通道数
        if (nb_channals == 1) {
            channaleConfig = AudioFormat.CHANNEL_OUT_MONO;
        } else if (nb_channals == 2) {
            channaleConfig = AudioFormat.CHANNEL_OUT_STEREO;
        } else {
            channaleConfig = AudioFormat.CHANNEL_OUT_MONO;
        }
        int buffersize = AudioTrack.getMinBufferSize(sampleRateInHz,
                channaleConfig, AudioFormat.ENCODING_PCM_16BIT);
        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRateInHz, channaleConfig,
                AudioFormat.ENCODING_PCM_16BIT, buffersize, AudioTrack.MODE_STREAM);
        audioTrack.play();
    }

    //C传入音频数据
    public void playTrack(byte[] buffer, int lenth) {
        if (audioTrack != null && audioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
            audioTrack.write(buffer, 0, lenth);
        }
    }
}
