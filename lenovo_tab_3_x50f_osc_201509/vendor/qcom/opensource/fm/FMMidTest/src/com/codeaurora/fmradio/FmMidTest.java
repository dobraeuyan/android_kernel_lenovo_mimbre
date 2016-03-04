/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information and source code
 *  contained herein is confidential. The software including the source code
 *  may not be copied and the information contained herein may not be used or
 *  disclosed except with the written permission of MEMSIC Inc. (C) 2009
 *****************************************************************************/
package com.codeaurora.fmradio;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import android.os.SystemProperties;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.view.View;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.view.KeyEvent;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.content.BroadcastReceiver;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.widget.Toast;
import android.view.Gravity;
import android.util.Log;
import android.media.MediaRecorder;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioSystem;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.os.Build;
import android.bluetooth.BluetoothAdapter;
import qcom.fmradio.FmReceiver;
import qcom.fmradio.FmRxEvCallbacksAdaptor;
import qcom.fmradio.FmConfig;
import android.os.Handler;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface.OnKeyListener;


public class FmMidTest extends Activity implements OnSeekBarChangeListener{
    /** Called when the activity is first created. */
	private SeekBar mSeekBar;   
	private TextView mFreqInfo, mHeadSetInfo;
	private AlertDialog mDialog = null;
	private Button local_ok_button;
	private Button local_error_button;
	private Button local_plus_button;
	private Button local_reset_button;
	private Button local_minus_button;
	public static int mDefaultFreq =  103700;
	private static  int mLocalFreq;
	private static int SEEKBAR_MAX_SCALE = 205;
	static int mVal;

    private int mTunedFrequency = 103700;
    private final Handler mHandler = new Handler();
    private static boolean mIsSeeking = false;
    private ProgressDialog mProgressDialog=null;

    private static final int DIALOG_PROGRESS_PROGRESS = 1;

	private boolean mHeadsetPlugged = false;
	private static final String FMRADIO_DEVICE_FD_STRING = "/dev/radio0";
	private FmReceiver mFmReceiver;
	private static FmSharedPreferences mPrefs;
	private static int mLowBand;
	private static int mUpBand;
	private boolean mFMOn = false;
	AudioManager am;
	private int mSystemVol = 0;
	private boolean mMuted = false;
	private boolean mIsRecording = false;
	Thread mRecordThread; 
	private BroadcastReceiver mHeadsetReceiver = null;
	public static final String LOGTAG = "FmMidTest";
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fm_midtest);
		mSeekBar = (SeekBar) this.findViewById(R.id.SeekBar01);
		mFreqInfo = (TextView) this.findViewById(R.id.TextView1);
		mHeadSetInfo = (TextView) this.findViewById(R.id.info);
		mSeekBar.setOnSeekBarChangeListener(this);
		mSeekBar.setMax(SEEKBAR_MAX_SCALE);
		local_ok_button = (Button)findViewById(R.id.ok_button);
		local_error_button = (Button)findViewById(R.id.error_button);
		local_plus_button = (Button)findViewById(R.id.plus);
		local_reset_button = (Button)findViewById(R.id.reset);
		local_minus_button = (Button)findViewById(R.id.minus);
		local_ok_button.setOnClickListener(listener);
		local_error_button.setOnClickListener(listener2);
		local_plus_button.setOnClickListener(plus);
		local_minus_button.setOnClickListener(minus);
		local_reset_button.setOnClickListener(reset);
		mSeekBar.setEnabled(false);
		local_plus_button.setEnabled(false);
		local_reset_button.setEnabled(false);
		local_minus_button.setEnabled(false);
		local_ok_button.setEnabled(false);
		setTitle(getString(R.string.radio_test).toString());
		mHeadSetInfo.setText(getString(R.string.headset_not_plug_in).toString());
		registerHeadsetListener();
		mPrefs = new FmSharedPreferences(this);
		mLowBand = FmSharedPreferences.getFMConfiguration().getLowerLimit();
		mUpBand = FmSharedPreferences.getFMConfiguration().getUpperLimit();

		SEEKBAR_MAX_SCALE = 205*(mUpBand - mLowBand)/(108000-87500);
		Log.d(LOGTAG,"onCreate SEEKBAR_MAX_SCALE="+SEEKBAR_MAX_SCALE+" ,mLowBand="+mLowBand+" ,mUpBand="+mUpBand);
		mVal = SEEKBAR_MAX_SCALE * (mDefaultFreq - mLowBand) / (mUpBand - mLowBand);
		mFreqInfo.setText(""+(double)(mDefaultFreq/1000.0));
		mSeekBar.setProgress(mVal);
		mSeekBar.setOnSeekBarChangeListener(this);
		am= (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		mSystemVol = am.getStreamVolume(AudioManager.STREAM_MUSIC);
		int vol_max = am.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		am.setStreamVolume(AudioManager.STREAM_MUSIC, vol_max / 2, 0);
		AudioSystem.setForceUse(AudioSystem.FOR_MEDIA, AudioSystem.FORCE_NONE);

	}

    private Handler mEnableRadioHandler = new Handler();
    private Runnable mEnableRadioTask = new Runnable() {
        public void run() {
            mLocalFreq = mDefaultFreq;
            fmOn();
            tune(mLocalFreq);
        }
    };

	@Override
	public void onResume() {
		super.onResume();
		mEnableRadioHandler.postDelayed(mEnableRadioTask, 500);
	}
	
	@Override
	public void onDestroy() {
		am.setStreamVolume(AudioManager.STREAM_MUSIC, mSystemVol, 0);
		super.onDestroy();
	}

	public void destroy(int result) {
	  mEnableRadioHandler.removeCallbacks(mEnableRadioTask);
	  AudioSystem.setDeviceConnectionState(AudioSystem.DEVICE_OUT_FM,
	   AudioSystem.DEVICE_STATE_UNAVAILABLE, "");
	  fmOff();
	  if (mHeadsetReceiver != null) {
	   unregisterReceiver(mHeadsetReceiver);
	   mHeadsetReceiver = null;
	  }
	  Intent mIntent = new Intent();
	  setResult(result, mIntent);
	  finish();
	}
	OnClickListener plus = new OnClickListener(){
		public void onClick(View v){
			int mFrequency;
			if (isFmOn() && mFmReceiver != null){
				mFmReceiver.searchStations(FmReceiver.FM_RX_SRCH_MODE_SEEK,
					FmReceiver.FM_RX_DWELL_PERIOD_1S,
					FmReceiver.FM_RX_SEARCHDIR_UP);
				// caoyang_20131106 bug159147 Modify @{
				/*mFrequency =  mFmReceiver.getTunedFrequency();
				double frequency = mFrequency / 1000.0;
				mFreqInfo.setText(""+frequency);
				mVal = SEEKBAR_MAX_SCALE * (mFrequency - mLowBand) / (mUpBand - mLowBand);
				mSeekBar.setProgress(mVal);
				Log.d(LOGTAG,"plus mFrequency="+mFrequency+" ,frequency="+frequency+" ,mVal="+mVal);
				*/
				mIsSeeking = true;
				updateSearchProgress();
				// @}
			}
		        
		}
	};
	OnClickListener reset = new OnClickListener(){
		public void onClick(View v){
			mLocalFreq = mDefaultFreq;
			mVal = SEEKBAR_MAX_SCALE * (mLocalFreq - mLowBand) / (mUpBand - mLowBand);
			mFreqInfo.setText(""+(double)(mLocalFreq/1000.0));
			mSeekBar.setProgress(mVal);
			tune(mLocalFreq);
			//Log.d(LOGTAG,"reset mLocalFreq="+mLocalFreq+" ,mFreqInfo="+mLocalFreq/1000.0+" ,mVal="+mVal);
		}
	};
	OnClickListener minus = new OnClickListener(){
		public void onClick(View v){
			int mFrequency;
			if (isFmOn() && mFmReceiver != null){
				mFmReceiver.searchStations(FmReceiver.FM_RX_SRCH_MODE_SEEK,
					FmReceiver.FM_RX_DWELL_PERIOD_1S,
					FmReceiver.FM_RX_SEARCHDIR_DOWN);
				// caoyang_20131106 bug159147 Modify @{
				/*mFrequency =  mFmReceiver.getTunedFrequency();
				double frequency = mFrequency / 1000.0;
				mFreqInfo.setText(""+frequency);
				mVal = SEEKBAR_MAX_SCALE * (mFrequency - mLowBand) / (mUpBand - mLowBand);
				mSeekBar.setProgress(mVal);
				//Log.d(LOGTAG,"minus mFrequency="+mFrequency+" ,frequency="+frequency+" ,mVal="+mVal);
				*/
				mIsSeeking = true;
				updateSearchProgress();
				// @}
			}
		}
	};

	OnClickListener listener = new OnClickListener(){
		public void onClick(View v){
			destroy(1);
		}
	};
	OnClickListener listener2 = new OnClickListener(){
		public void onClick(View v){
			destroy(-1);
		}
	};

	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		mVal = progress;
		mLocalFreq = mVal * (mUpBand - mLowBand) / SEEKBAR_MAX_SCALE + mLowBand;
		Log.d(LOGTAG,"onProgressChanged mUpBand="+mUpBand+" ,mLowBand="+mLowBand+" ,SEEKBAR_MAX_SCALE="+SEEKBAR_MAX_SCALE);
		mFreqInfo.setText(""+ (double)(mLocalFreq/1000.0));
		//Log.d(LOGTAG,"onProgressChanged mLocalFreq="+mLocalFreq+" ,mFreqInfo="+(double)(mLocalFreq/1000.0)+" ,mVal="+mVal);
	}

	public void onStartTrackingTouch(SeekBar seekBar) {
	}
	public void onStopTrackingTouch(SeekBar seekBar) {
		tune(mLocalFreq);
		Log.d(LOGTAG,"onStopTrackingTouch mLocalFreq="+mLocalFreq);
	}

	//ignore keys of back, focus, camera, volume up and down fluence;
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		switch (event.getKeyCode()) {
			case KeyEvent.KEYCODE_HEADSETHOOK:
			case KeyEvent.KEYCODE_HOME:
			case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
			case KeyEvent.KEYCODE_ENTER:
			case KeyEvent.KEYCODE_POWER:
			case KeyEvent.KEYCODE_CAMERA:
			case KeyEvent.KEYCODE_FOCUS:
			case KeyEvent.KEYCODE_SEARCH:
			case KeyEvent.KEYCODE_VOLUME_DOWN:
			case KeyEvent.KEYCODE_VOLUME_UP:
			case KeyEvent.KEYCODE_ENDCALL:
			case KeyEvent.KEYCODE_CALL:
			case KeyEvent.KEYCODE_DPAD_DOWN:
			case KeyEvent.KEYCODE_DPAD_UP:
			case KeyEvent.KEYCODE_DPAD_RIGHT:
			case KeyEvent.KEYCODE_DPAD_LEFT:
			case KeyEvent.KEYCODE_DPAD_CENTER:
				return true;
			default:
				break;
		}
		return super.dispatchKeyEvent(event);
	}


    public void registerHeadsetListener() {
        if (mHeadsetReceiver == null) {
            mHeadsetReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    String action = intent.getAction();
		            if (action.equals(Intent.ACTION_HEADSET_PLUG)) {
		               Log.d(LOGTAG, "ACTION_HEADSET_PLUG Intent received");
		               Log.d(LOGTAG, "mReceiver: ACTION_HEADSET_PLUG");
		               Log.d(LOGTAG, "    state: " + intent.getIntExtra("state", 0));
		               mHeadsetPlugged = (intent.getIntExtra("state", 0) == 1);
					   if(mHeadsetPlugged){
						mSeekBar.setEnabled(true);
						local_plus_button.setEnabled(true);
						local_reset_button.setEnabled(true);
						local_minus_button.setEnabled(true);
						 local_ok_button.setEnabled(true);
						 mHeadSetInfo.setText(getString(R.string.headset_pluged_in).toString());
						 mHeadSetInfo.setTextColor(0xffff0000);
						 unMute();
					   }else{
						mSeekBar.setEnabled(false);
						local_plus_button.setEnabled(false);
						local_reset_button.setEnabled(false);
						local_minus_button.setEnabled(false);
						  local_ok_button.setEnabled(false);
					  	  mHeadSetInfo.setText(getString(R.string.headset_not_plug_in).toString());
						  mHeadSetInfo.setTextColor(0xffff0000);
					  	  mute();
					   }
                    }
                }
            };
            IntentFilter iFilter = new IntentFilter();
            iFilter.addAction(Intent.ACTION_HEADSET_PLUG);
            registerReceiver(mHeadsetReceiver, iFilter);
        }
    }

	protected AlertDialog show_dialog()
	{
		return new AlertDialog.Builder(this)
	         .setTitle(getString(R.string.midtest))
	         .setCancelable(true)
                .setMessage(getString(R.string.headset_not_plug_in))
                .setNeutralButton(getString(R.string.confirm),
                        new DialogInterface.OnClickListener(){
                            public void onClick(DialogInterface d, int w) {
								mDialog = null;

                            }
                        })
		.show();
	}

  /* Returns whether FM hardware is ON.
   *
   * @return true if FM was tuned, searching. (at the end of
   * the search FM goes back to tuned).
   *
   */
  public boolean isFmOn() {
	 return mFMOn;
  }

  private void startFM(){
	Log.d(LOGTAG, "In startFM");
	mIsRecording = true;
}

   private void stopFM(){
	Log.d(LOGTAG, "In stopFM");
	mIsRecording = false;
   }

  private void stop() {
      mFMOn = false;
   }
	 
  /*
   * Turn ON FM: Powers up FM hardware, and initializes the FM module
   *                                                                                 .
   * @return true if fm Enable api was invoked successfully, false if the api failed.
   */
	private boolean fmOn() {
		boolean bStatus=false;

		if(mFmReceiver == null){
			try {
				mFmReceiver = new FmReceiver(FMRADIO_DEVICE_FD_STRING, fmCallbacks);
			}catch (InstantiationException e){
				destroy(-2);
			}
		}

		if (mFmReceiver != null){
			if (isFmOn()){
				/* FM Is already on,*/
				bStatus = true;
				Log.d(LOGTAG, "mReceiver.already enabled");
			}else{
				// This sets up the FM radio device
				FmConfig config = mPrefs.getFMConfiguration();
				bStatus = mFmReceiver.enable(config);
				bStatus = true;
				Log.d(LOGTAG, "mReceiver.enable done, Status :" +  bStatus);
			}
			if (bStatus == true){
				AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
				if(audioManager != null){
					Log.d(LOGTAG, "mAudioManager.setFmRadioOn = true \n" );
					audioManager.setParameters("Fm-radio=true");
					//mFmReceiver.setAnalogMode(true);
					Log.d(LOGTAG,"fmon am.isWiredHeadsetOn()="+am.isWiredHeadsetOn());
					if(am.isWiredHeadsetOn()) {
					   AudioSystem.setDeviceConnectionState(AudioSystem.DEVICE_OUT_FM, AudioSystem.DEVICE_STATE_AVAILABLE, "");
					}
					startFM();
					Log.d(LOGTAG, "mAudioManager.setFmRadioOn done \n" );
				}
				bStatus = true;
			}else{
				stop();
			}
			mFMOn = true;
		}
		return(bStatus);
	}
  
  /*
   * Turn OFF FM: Disable the FM Host and hardware                                  .
   *                                                                                 .
   * @return true if fm Disable api was invoked successfully, false if the api failed.
   */
   private boolean fmOff() {
      boolean bStatus=false;

      stopFM();
	AudioSystem.setForceUse(AudioSystem.FOR_MEDIA, AudioSystem.FORCE_NONE);
	SystemProperties.set("hw.fm.isAnalog","false");

      AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
      if(audioManager != null)
      {
         Log.d(LOGTAG, "audioManager.setFmRadioOn = false \n" );
        audioManager.setParameters("Fm-radio=false");
         //audioManager.setParameters("FMRadioOn=false");
         Log.d(LOGTAG, "audioManager.setFmRadioOn false done \n" );
      }
      // This will disable the FM radio device
      if (mFmReceiver != null)
      {
         bStatus = mFmReceiver.disable();
         mFmReceiver = null;
      }
      stop();
      return(bStatus);
   }

   /* Tunes to the specified frequency
    *
    * @return true if Tune command was invoked successfully, false if not muted.
    *  Note: Callback FmRxEvRadioTuneStatus will be called when the tune
    *        is complete
    * 108000khz = 108Mhz
    */
   public boolean tune(int frequency) {
      boolean bCommandSent=false;
      double doubleFrequency = frequency/1000.00;

      Log.d(LOGTAG, "tuneRadio:  " + doubleFrequency);
      if (mFmReceiver != null)
      {
         mFmReceiver.setStation(frequency);
         bCommandSent = true;
      }
      return bCommandSent;
   }
   
   /*
    *  Mute FM Hardware (SoC)
    * @return true if set mute mode api was invoked successfully, false if the api failed.
    */
    public boolean mute() {
       boolean bCommandSent=false;
       Log.d(LOGTAG, "mute:");
       if (mFmReceiver != null)
       {
          mMuted = true;
          bCommandSent = mFmReceiver.setMuteMode(FmReceiver.FM_RX_MUTE);
	AudioSystem.setDeviceConnectionState(AudioSystem.DEVICE_OUT_FM, AudioSystem.DEVICE_STATE_UNAVAILABLE, "");//MT a call, ringtone comes from both headset and speaker  
       }
       return bCommandSent;
    }

    /*
     *  UnMute FM Hardware (SoC)
     * @return true if set mute mode api was invoked successfully, false if the api failed.
     */
     public boolean unMute() {
        boolean bCommandSent=false;
        Log.d(LOGTAG, "unMute:");
        if (mFmReceiver != null)
        {
           mMuted = false;
           bCommandSent = mFmReceiver.setMuteMode(FmReceiver.FM_RX_UNMUTE);
	AudioSystem.setDeviceConnectionState(AudioSystem.DEVICE_OUT_FM, AudioSystem.DEVICE_STATE_AVAILABLE, "");
        }
        return bCommandSent;
     }
     
   /* Receiver callbacks back from the FM Stack */
   FmRxEvCallbacksAdaptor fmCallbacks = new FmRxEvCallbacksAdaptor()
   {
      public void FmRxEvEnableReceiver() {
         Log.d(LOGTAG, "FmRxEvEnableReceiver");
      }
      public void FmRxEvDisableReceiver()
      {
         Log.d(LOGTAG, "FmRxEvEnableReceiver");
      }
        public void FmRxEvSearchComplete(int frequency) {
            Log.d(LOGTAG, "FmMidTest FmRxEvSearchComplete: Tuned Frequency: " +frequency);
            mTunedFrequency = frequency;
            mHandler.post(mSearchComplete);
        }
        public void FmRxEvRadioTuneStatus(int frequency) {
            Log.d(LOGTAG, "FmMidTest FmRxEvRadioTuneStatus: Tuned Frequency: " +frequency);
            mTunedFrequency = frequency;
            mHandler.post(mUpdateStationInfo);
        }
   };

    final Runnable mSearchComplete = new Runnable() {
        public void run() {
            Log.d(LOGTAG, "FmMidTest mSearchComplete: ");
            mIsSeeking = false;
            updateSearchProgress();
            double frequency = mTunedFrequency / 1000.0;
            mFreqInfo.setText(""+frequency);
            mVal = SEEKBAR_MAX_SCALE * (mTunedFrequency - mLowBand) / (mUpBand - mLowBand);
            mSeekBar.setProgress(mVal);
        }
    };

    final Runnable mUpdateStationInfo = new Runnable() {
        public void run() {
            updateSearchProgress();
            double frequency = mTunedFrequency / 1000.0;
            mFreqInfo.setText(""+frequency);
            mVal = SEEKBAR_MAX_SCALE * (mTunedFrequency - mLowBand) / (mUpBand - mLowBand);
            mSeekBar.setProgress(mVal);
        }
    };

    private void updateSearchProgress() {
        boolean searchActive = mIsSeeking;
        if(searchActive) {
            synchronized (this) {
                if(mProgressDialog == null) {
                    showDialog(DIALOG_PROGRESS_PROGRESS);
                }
            }
        } else {
            removeDialog(DIALOG_PROGRESS_PROGRESS);
            mProgressDialog=null;
        }
    }

    @Override
    protected Dialog onCreateDialog(int id) {
        AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);
        dlgBuilder.setOnKeyListener(new OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
                Log.d(LOGTAG, "OnKeyListener event received"+keyCode);
                switch (keyCode) {
                    case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                    case 126: //KeyEvent.KEYCODE_MEDIA_PLAY:
                    case 127: //KeyEvent.KEYCODE_MEDIA_PAUSE:
                    case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
                    case KeyEvent.KEYCODE_MEDIA_NEXT:
                    case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
                    case KeyEvent.KEYCODE_MEDIA_REWIND:
                    case KeyEvent.KEYCODE_MEDIA_STOP:
                    return true;
                }
                return false;
            }
        });
        switch (id) {
            case DIALOG_PROGRESS_PROGRESS: {
                return createProgressDialog(id);
            }
            default:
                break;
        }
        return null;
    }

    private Dialog createProgressDialog(int id) {
        String msgStr="";
        String titleStr="";
        boolean bSearchActive = false;

        if(mIsSeeking) {
            msgStr = getString(R.string.msg_seeking);
            bSearchActive=true;
        }

        if(bSearchActive) {
            mProgressDialog = new ProgressDialog(FmMidTest.this);
            if (mProgressDialog != null) {
                mProgressDialog.setTitle(titleStr);
                mProgressDialog.setMessage(msgStr);
                mProgressDialog.setIcon(R.drawable.ic_launcher_fmradio);
                mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
                mProgressDialog.setCanceledOnTouchOutside(false);
                mProgressDialog.setButton(DialogInterface.BUTTON_POSITIVE,
                        getText(R.string.button_text_stop),
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int whichButton) {
                                if (mFmReceiver != null) {
                                    mFmReceiver.cancelSearch();
                                }
                            }
                        }
                );
                mProgressDialog.setOnCancelListener(new DialogInterface.OnCancelListener() {
                    public void onCancel(DialogInterface dialog) {
                        if (mFmReceiver != null) {
                            mFmReceiver.cancelSearch();
                        }
                    }
                });
                mProgressDialog.setOnKeyListener(new OnKeyListener() {
                    public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
                        Log.d(LOGTAG, "OnKeyListener event received in ProgressDialog"+keyCode);
                        switch (keyCode) {
                        case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                        case 126: //KeyEvent.KEYCODE_MEDIA_PLAY:
                        case 127: //KeyEvent.KEYCODE_MEDIA_PAUSE:
                        case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
                        case KeyEvent.KEYCODE_MEDIA_NEXT:
                        case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
                        case KeyEvent.KEYCODE_MEDIA_REWIND:
                        case KeyEvent.KEYCODE_MEDIA_STOP:
                            return true;
                        }
                        return false;
                    }
                });
            }
        }
        return mProgressDialog;
    }

}
