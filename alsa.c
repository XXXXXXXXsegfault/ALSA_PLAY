struct snd_mask
{
	unsigned int bits[8];
};
struct snd_interval
{
	unsigned int min;
	unsigned int max;
	unsigned int openmin:1;
	unsigned int openmax:1;
	unsigned int integer:1;
	unsigned int empty:1;
};
struct snd_pcm_hw_params
{
	unsigned int flags;
	struct snd_mask masks[3];
	struct snd_mask mres[5];
	struct snd_interval intervals[12];
	struct snd_interval ires[9];
	unsigned int rmask;
	unsigned int cmask;
	unsigned int info;
	unsigned int msbits;
	unsigned int rate_num;
	unsigned int rate_den;
	unsigned int fifo_size;
	unsigned char reserved[64];
};
struct snd_xferi
{
	long int result;
	void *buf;
	unsigned long int frames;
};
#define SNDRV_CTL_IOCTL_PCM_PREFER_SUBDEVICE 0x40045532
#define SNDRV_PCM_IOCTL_HW_REFINE 0xc2604110
#define SNDRV_PCM_IOCTL_HW_PARAMS 0xc2604111
#define SNDRV_PCM_IOCTL_PREPARE 0x4140
#define SNDRV_PCM_IOCTL_WRITEI_FRAMES 0x40184150
struct snd_pcm_dev
{
	int ctrlfd;
	int pcmfd;
};
int snd_pcm_open(int ctrl,int dev,char *type,struct snd_pcm_dev *ret)
{
	int fd;
	char devname[32];
	strcpy(devname,"/dev/snd/controlC");
	iappendd(devname,ctrl,1,10);
	if((fd=sys_open(devname,02000002,0))<0)
	{
		return -1;
	}
	if(sys_ioctl(fd,SNDRV_CTL_IOCTL_PCM_PREFER_SUBDEVICE,&dev))
	{
		sys_close(fd);
		return -2;
	}
	ret->ctrlfd=fd;
	strcpy(devname,"/dev/snd/pcmC");
	iappendd(devname,ctrl,1,10);
	strcat(devname,"D");
	iappendd(devname,dev,1,10);
	strcat(devname,type);
	if((fd=sys_open(devname,02004002,0))<0)
	{
		sys_close(ret->ctrlfd);
		return -3;
	}
	if(sys_fcntl(fd,4,02000002))
	{
		sys_close(fd);
		sys_close(ret->ctrlfd);
		return -4;
	}
	ret->pcmfd=fd;
	return 0;
}
#define snd_pcm_close(dev) (sys_close((dev)->ctrlfd),sys_close((dev)->pcmfd))
int snd_pcm_hw_params_init(struct snd_pcm_dev *dev,struct snd_pcm_hw_params *params)
{
	int x=1;
	memset(params,0,sizeof(struct snd_pcm_hw_params));
	params->masks[0].bits[0]|=8;
	while(x<=2)
	{
		memset(params->masks+x,0xff,sizeof(struct snd_mask));
		x++;
	}
	x=0;
	while(x<=11)
	{
		params->intervals[x].max=0xffffffff;
		x++;
	}
	params->rmask=0xffffffff;
	params->cmask=0;
	params->info=0xffffffff;
	return sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_HW_REFINE,params);
}
int snd_pcm_hw_params_set_format(struct snd_pcm_dev *dev,struct snd_pcm_hw_params *params,int format)
{
	unsigned int val=params->masks[1].bits[format>>5]&1<<(format&31);
	if(!val)
	{
		return 1;
	}
	memset(params->masks+1,0,sizeof(struct snd_mask));
	params->masks[1].bits[format>>5]=val;
	params->rmask|=2U;
	return sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_HW_REFINE,params);
}
int snd_pcm_hw_params_set_rate(struct snd_pcm_dev *dev,struct snd_pcm_hw_params *params,unsigned int rate)
{
	params->intervals[3].min=rate;
	params->intervals[3].max=rate;
	params->intervals[3].openmin=0;
	params->intervals[3].openmax=0;
	params->intervals[3].integer=0;
	params->intervals[3].empty=0;
	params->rmask|=2048U;
	return sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_HW_REFINE,params);
}
int snd_pcm_hw_params_set_channels(struct snd_pcm_dev *dev,struct snd_pcm_hw_params *params,unsigned int channels)
{
	params->intervals[2].min=channels;
	params->intervals[2].max=channels;
	params->intervals[2].openmin=0;
	params->intervals[2].openmax=0;
	params->intervals[2].integer=0;
	params->intervals[2].empty=0;
	params->rmask|=1024U;
	return sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_HW_REFINE,params);
}
#define snd_pcm_hw_params_set(dev,params) sys_ioctl((dev)->pcmfd,SNDRV_PCM_IOCTL_HW_PARAMS,params)
int snd_pcm_write(struct snd_pcm_dev *dev,void *buf,unsigned int frames)
{
	struct snd_xferi frame={0,buf,frames};
	sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_PREPARE,0);
	return sys_ioctl(dev->pcmfd,SNDRV_PCM_IOCTL_WRITEI_FRAMES,&frame);
}
