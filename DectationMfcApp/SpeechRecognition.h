#include <opencv2\opencv.hpp>
#include <pxcsession.h>
#include <pxcaudiosource.h>
#include <pxcspeechrecognition.h>

class SpeechRecognition : public PXCSpeechRecognition::Handler
{
public:
	SpeechRecognition::SpeechRecognition()
	{
		InitializeAudioDevice();
	}

private:
	void InitializeAudioDevice()
	{
		PXCSession *session = PXCSession_Create();
		PXCAudioSource *audioSource = session->CreateAudioSource();

		PXCAudioSource::DeviceInfo device = {};
		audioSource->ScanDevices();

		auto deviceNum = audioSource->QueryDeviceNum();
		for (int i = 0; i < deviceNum; i++)
		{
			PXCAudioSource::DeviceInfo dinfo = {};
			auto sts = audioSource->QueryDeviceInfo(i, &dinfo);
			if (sts < PXC_STATUS_NO_ERROR)
			{
				throw std::runtime_error("Fail Getting Device Info");
			}

			std::wcout << "\t" << dinfo.name;

			if (i == 0)
			{
				device = dinfo;
			}
		}

		auto sts = audioSource->SetDevice(&device);
		if (sts < PXC_STATUS_NO_ERROR)
		{
			throw std::runtime_error("Fail Set Audio Device.");
		}

		PXCSession::ImplDesc in = {};
		PXCSession::ImplDesc out = {};
		PXCSession::ImplDesc desc = {};
		in.cuids[0] = PXCSpeechRecognition::CUID;

		for (int i = 0;; i++)
		{
			auto sts = session->QueryImpl(&in, i, &out);
			if (sts < PXC_STATUS_NO_ERROR)
			{
				break;
			}
			std::wcout << "\t" << out.friendlyName;
			if (i == 0)
			{
				desc = out;
			}
		}

		PXCSpeechRecognition* recognition;
		sts = session->CreateImpl<PXCSpeechRecognition>(&desc, &recognition);

		PXCSpeechRecognition::ProfileInfo profile = {};
		for (int j = 0;; j++)
		{
			PXCSpeechRecognition::ProfileInfo pinfo;
			sts = recognition->QueryProfile(j, &pinfo);
			if (sts < PXC_STATUS_NO_ERROR)
			{
				break;
			}

			if (pinfo.language == PXCSpeechRecognition::LANGUAGE_JP_JAPANESE)
			{
				profile = pinfo;
			}
		}

		sts = recognition->SetProfile(&profile);
		recognition->SetDictation();
		sts = recognition->StartRec(audioSource, this);
	}

	virtual void PXCAPI OnRecognition(const PXCSpeechRecognition::RecognitionData * data)
	{
		::MessageBox(NULL, data->scores[0].sentence, NULL, MB_OK);
	}
};

