#ifndef INTEL_DUAL_DISPLAY_H
#define INTEL_DUAL_DISPLAY_H
#include <drm/drmP.h> 

//#define CONFIG_MIPI_DLP		 //lenovo, tanhua1, 20150331, enable mipi DLP for 13A, this config is define in the BOARD_DEFCONFIG

//#define CONFIG_HDMI_DLP	//lenovo, tanhua1, 20150331, enable HDMI  DLP for 10A, this config is define in the BOARD_DEFCONFIG

#if defined (CONFIG_MIPI_DLP) || defined (CONFIG_HDMI_DLP)
/*2014-06-05 intel require don't change the order*/
enum dual_display_status_type {
    PIPE_INIT = 109,
    PIPE_OFF =110,
    PIPE_ON =111,
    PIPE_ON_PROCESSING =112,
    PIPE_OFF_PROCESSING =113
};
struct intel_dual_display_status{	
	enum dual_display_status_type primary_status;
	enum dual_display_status_type external_status;
	struct mutex dual_display_mutex;	/**< For others */
       bool dual_display_enabled;
       struct delayed_work delay_work;
       struct workqueue_struct *dlpq;
	bool (*call_back)(int);
};
int simulate_valleyview_irq_handler(bool enable,bool (*cb)(int));
//extern struct intel_dual_display_status dual_display_status;
void dual_display_delay_work(struct work_struct *work);

#endif

#if defined (CONFIG_MIPI_DLP)
#define PRIMARY_PIPE PIPE_B
#define EXTERNAL_PIPE PIPE_A
#elif defined CONFIG_HDMI_DLP
#define PRIMARY_PIPE PIPE_A
#define EXTERNAL_PIPE PIPE_C
#endif
#endif
