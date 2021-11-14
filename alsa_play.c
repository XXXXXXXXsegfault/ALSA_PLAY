asm(".text");
asm(".globl _start");
asm("_start:");
asm("mov (%rsp),%rdi");
asm("lea 8(%rsp),%rsi");
asm("call main");
asm("mov %rax,%rdi");
asm("mov $231,%eax");
asm("syscall");
#include "syscall.c"
#include "mem.c"
#include "iappend.c"
#include "alsa.c"
int read_num(char *str)
{
	int result=0;
	int x=0;
	char c;
	while(c=str[x])
	{
		if(c>='0'&&c<='9')
		{
			result=result*10+(c-'0');
		}
		else
		{
			break;
		}
		x++;
	}
	return result;
}
char audio_buf[3072];
int frame_size[14]={1,1,2,2,2,2,3,3,3,3,4,4,4,4};
int main(int argc,char *argv[])
{
	struct snd_pcm_dev sdev;
	struct snd_pcm_hw_params params;
	int ctrl,dev;
	int sample_rate,channels,format;
	int file;
	int n;
	if(argc<5)
	{
		sys_write(2,"Usage: alsa_play [sample rate] [channels] [format] [file]\n",58);
		sys_write(2,"Formats:\n",9);
		sys_write(2,"0 -- S8\n",8);
		sys_write(2,"1 -- U8\n",8);
		sys_write(2,"2 -- S16_LE\n",12);
		sys_write(2,"3 -- S16_BE\n",12);
		sys_write(2,"4 -- U16_LE\n",12);
		sys_write(2,"5 -- U16_BE\n",12);
		sys_write(2,"6 -- S24_LE\n",12);
		sys_write(2,"7 -- S24_BE\n",12);
		sys_write(2,"8 -- U24_LE\n",12);
		sys_write(2,"9 -- U24_BE\n",12);
		sys_write(2,"10 -- S32_LE\n",13);
		sys_write(2,"11 -- S32_BE\n",13);
		sys_write(2,"12 -- U32_LE\n",13);
		sys_write(2,"13 -- U32_BE\n",13);
		return 1;
	}
	sample_rate=read_num(argv[1]);
	channels=read_num(argv[2]);
	format=read_num(argv[3]);
	if(format<0||format>=14)
	{
		sys_write(2,"Invalid format.\n",16);
		return 1;
	}
	if((file=sys_open(argv[4],0,0))<0)
	{
		sys_write(2,"Cannot open file.\n",18);
		return 1;
	}
	for(ctrl=0;ctrl<16;ctrl++)
	{
		for(dev=0;dev<16;dev++)
		{
			if(!snd_pcm_open(ctrl,dev,"p",&sdev))
			{
				goto Open_success;
			}
		}
	}
	sys_write(2,"Cannot open ALSA device.\n",25);
	sys_close(file);
	return 1;
Open_success:
	if(snd_pcm_hw_params_init(&sdev,&params))
	{
		sys_write(2,"Cannot initialize HWPARAMS.\n",28);
		snd_pcm_close(&sdev);
		sys_close(file);
		return 1;
	}
	if(snd_pcm_hw_params_set_rate(&sdev,&params,sample_rate))
	{
		sys_write(2,"Cannot set sample rate.\n",24);
		snd_pcm_close(&sdev);
		sys_close(file);
		return 1;
	}
	if(snd_pcm_hw_params_set_channels(&sdev,&params,channels))
	{
		sys_write(2,"Cannot set channels.\n",21);
		snd_pcm_close(&sdev);
		sys_close(file);
		return 1;
	}
	if(snd_pcm_hw_params_set_format(&sdev,&params,format))
	{
		sys_write(2,"Cannot set format.\n",19);
		snd_pcm_close(&sdev);
		sys_close(file);
		return 1;
	}
	if(snd_pcm_hw_params_set(&sdev,&params))
	{
		sys_write(2,"Cannot set HWPARAMS.\n",21);
		snd_pcm_close(&sdev);
		sys_close(file);
		return 1;
	}
	while((n=sys_read(file,audio_buf,3072))>0)
	{
		snd_pcm_write(&sdev,audio_buf,n/(frame_size[format]*channels));
	}
	snd_pcm_close(&sdev);
	sys_close(file);
	return 0;
}
