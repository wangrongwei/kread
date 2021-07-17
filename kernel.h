#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "common.h"

#define VA_BITS 48
#define VA_BITS_ACTUAL 48

/* 
 * sources: Documentation/arm64/memory.txt 
 *          arch/arm64/include/asm/memory.h 
 *          arch/arm64/include/asm/pgtable.h
 */
#define ARM64_VA_START       ((0xffffffffffffffffUL) \
	<< VA_BITS)
#define _VA_START(va)        ((0xffffffffffffffffUL) - \
	((1UL) << ((va) - 1)) + 1)
#define TEXT_OFFSET_MASK     (~((MEGABYTES(2UL))-1))
#define ARM64_PAGE_OFFSET    ((0xffffffffffffffffUL) \
	<< (VA_BITS - 1))
#define ARM64_PAGE_OFFSET_ACTUAL ((0xffffffffffffffffUL) \
	- ((1UL) << VA_BITS_ACTUAL) + 1)

#define ARM64_USERSPACE_TOP  ((1UL) << VA_BITS)
#define ARM64_USERSPACE_TOP_ACTUAL  ((1UL) << VA_BITS_ACTUAL)
#define VA_START _VA_START(VA_BITS_ACTUAL)

#define ARM64_VMALLOC_END    (ARM64_PAGE_OFFSET - 0x400000000UL - KILOBYTES(64) - 1)
#define ARM64_VMEMMAP_VADDR  ((ARM64_VMALLOC_END+1) + KILOBYTES(64))
#define ARM64_VMEMMAP_END    (ARM64_VMEMMAP_VADDR + GIGABYTES(8UL) - 1)

/* only used for v4.6 or later */
#define ARM64_MODULES_VSIZE     MEGABYTES(128)
#define ARM64_KASAN_SHADOW_SIZE (1UL << (VA_BITS - 3))

#define TASK_COMM_LEN 16     /* task command name length including NULL */
struct task_context {       /* context stored for each task */
	unsigned long task;
	unsigned long thread_info;
	unsigned long pid;
	char comm[TASK_COMM_LEN+1];
	int processor;
	unsigned long ptask;
	unsigned long mm_struct;
	struct task_context *tc_next;
};

extern int generic_is_kvaddr(unsigned long addr);
extern int generic_is_uvaddr(unsigned long addr, struct task_context *tc);

#ifdef X86_64
extern int x86_kvtop(struct task_context *tc, unsigned long kvaddr, physaddr_t *paddr, int verbose);
#elif defined(ARM64)
extern int arm64_kvtop(struct task_context *tc, unsigned long kvaddr, physaddr_t *paddr, int verbose);
#endif

#define IS_KVADDR(X)       (generic_is_kvaddr(X))
#define IS_UVADDR(X,C)     (generic_is_uvaddr(X,C))
#define IS_VMALLOC_ADDR(X)    arm64_is_vmalloc_addr((unsigned long)(X))

/* kernel pgtable */
extern unsigned long kernel_pgd[];

struct offset_table {       /* stash of commonly-used offsets */
	long list_head_next; /* add new entries to end of table */
	long list_head_prev;
	long task_struct_pid;
	long task_struct_state;
	long task_struct_comm;
	long task_struct_mm;
	long task_struct_tss;
	long task_struct_thread;
	long task_struct_active_mm;
	long task_struct_tss_eip;
	long task_struct_tss_esp;
	long task_struct_tss_ksp;
	long task_struct_processor;
	long task_struct_p_pptr;
	long task_struct_parent;
	long task_struct_has_cpu;
	long task_struct_cpus_runnable;
	long task_struct_thread_eip;
	long task_struct_thread_esp;
	long task_struct_thread_ksp;
	long task_struct_next_task;
	long task_struct_files;
	long task_struct_fs;
	long task_struct_pidhash_next;
	long task_struct_next_run;
	long task_struct_flags;
	long task_struct_sig;
	long task_struct_signal;
	long task_struct_blocked;
	long task_struct_sigpending;
	long task_struct_pending;
	long task_struct_sigqueue;
	long task_struct_sighand;
	long task_struct_start_time;
	long task_struct_times;
	long task_struct_utime;
	long task_struct_stime;
	long task_struct_cpu;
	long task_struct_run_list;
	long task_struct_pgrp;
	long task_struct_tgid;
	long task_struct_namespace;
	long task_struct_pids;
	long task_struct_last_run;
	long task_struct_timestamp;
	long task_struct_thread_info;
	long task_struct_nsproxy;
	long task_struct_rlim;

	long task_struct_tasks; /* list_head */

	long thread_info_task;
	long thread_info_cpu;
	long thread_info_previous_esp;
	long thread_info_flags;
	long nsproxy_mnt_ns;
	long mnt_namespace_root;
	long mnt_namespace_list;
	long pid_link_pid;
	long pid_hash_chain;
	long hlist_node_next;
	long hlist_node_pprev;
	long pid_pid_chain;
	long thread_struct_eip;
	long thread_struct_esp;
	long thread_struct_ksp;
	long thread_struct_fph;
	long thread_struct_rip;
	long thread_struct_rsp;
	long thread_struct_rsp0;
	long tms_tms_utime;
	long tms_tms_stime;
	long signal_struct_count;
	long signal_struct_action;
	long signal_struct_shared_pending;
	long signal_struct_rlim;
	long k_sigaction_sa;
	long sigaction_sa_handler;
	long sigaction_sa_flags;
	long sigaction_sa_mask;
	long sigpending_head;
	long sigpending_list;
	long sigpending_signal;
	long signal_queue_next;
	long signal_queue_info;
	long sigqueue_next;
	long sigqueue_list;
	long sigqueue_info;
	long sighand_struct_action;
	long siginfo_si_signo;
	long thread_struct_cr3;
	long thread_struct_ptbr;
	long thread_struct_pg_tables;
	long switch_stack_r26;
	long switch_stack_b0;
	long switch_stack_ar_bspstore;
	long switch_stack_ar_pfs;
	long switch_stack_ar_rnat;
	long switch_stack_pr;
	long cpuinfo_ia64_proc_freq;
	long cpuinfo_ia64_unimpl_va_mask;
	long cpuinfo_ia64_unimpl_pa_mask;
	long device_node_type;
	long device_node_allnext;
	long device_node_properties;
	long property_name;
	long property_value;
	long property_next;
	long machdep_calls_setup_residual;
	long RESIDUAL_VitalProductData;
	long VPD_ProcessorHz;
	long bd_info_bi_intfreq;
	long hwrpb_struct_cycle_freq;
	long hwrpb_struct_processor_offset;
	long hwrpb_struct_processor_size;
	long percpu_struct_halt_PC;
	long percpu_struct_halt_ra;
	long percpu_struct_halt_pv;
	long mm_struct_mmap;
	long mm_struct_pgd;
	long mm_struct_rss;
	long mm_struct_anon_rss;
	long mm_struct_file_rss;
	long mm_struct_total_vm;
	long mm_struct_start_code;
	long mm_struct_arg_start;
	long mm_struct_arg_end;
	long mm_struct_env_start;
	long mm_struct_env_end;
	long vm_area_struct_vm_mm;
	long vm_area_struct_vm_next;
	long vm_area_struct_vm_end;
	long vm_area_struct_vm_start; 
	long vm_area_struct_vm_flags;
	long vm_area_struct_vm_file;
	long vm_area_struct_vm_offset;
	long vm_area_struct_vm_pgoff;
	long vm_struct_addr;
	long vm_struct_size;
	long vm_struct_next;
	long module_size_of_struct;
	long module_next;
	long module_size;
	long module_name;
	long module_nsyms;
	long module_syms;
	long module_flags;
	long module_num_syms;
	long module_list;
	long module_gpl_syms;
	long module_num_gpl_syms;
	long module_module_core;
	long module_core_size;
	long module_core_text_size;
	long module_num_symtab;
	long module_symtab;
	long module_strtab;

	long module_kallsyms_start;
	long kallsyms_header_sections;
	long kallsyms_header_section_off;
	long kallsyms_header_symbols;
	long kallsyms_header_symbol_off;
	long kallsyms_header_string_off;
	long kallsyms_symbol_section_off;
	long kallsyms_symbol_symbol_addr;
	long kallsyms_symbol_name_off;
	long kallsyms_section_start;
	long kallsyms_section_size;
	long kallsyms_section_name_off;

	long page_next;
	long page_prev;
	long page_next_hash;
	long page_list;
	long page_list_next;
	long page_list_prev;
	long page_inode;
	long page_offset;
	long page_count;
	long page_flags;
	long page_mapping;
	long page_index;
	long page_buffers;
	long page_lru;
	long page_pte;
	long swap_info_struct_swap_file;
	long swap_info_struct_swap_vfsmnt;
	long swap_info_struct_flags;
	long swap_info_struct_swap_map;
	long swap_info_struct_swap_device;
	long swap_info_struct_prio;
	long swap_info_struct_max;
	long swap_info_struct_pages;
	long swap_info_struct_old_block_size;
	long block_device_bd_inode;
	long block_device_bd_list;
	long block_device_bd_disk;
	long irq_desc_t_status;
	long irq_desc_t_handler;
	long irq_desc_t_chip;
	long irq_desc_t_action;
	long irq_desc_t_depth;
	long irqdesc_action;
	long irqdesc_ctl;
	long irqdesc_level;
	long irqaction_handler;
	long irqaction_flags;
	long irqaction_mask;
	long irqaction_name;
	long irqaction_dev_id;
	long irqaction_next;
	long hw_interrupt_type_typename;
	long hw_interrupt_type_startup;
	long hw_interrupt_type_shutdown;
	long hw_interrupt_type_handle;
	long hw_interrupt_type_enable;
	long hw_interrupt_type_disable;
	long hw_interrupt_type_ack;
	long hw_interrupt_type_end;
	long hw_interrupt_type_set_affinity;
	long irq_chip_typename;
	long irq_chip_startup;
	long irq_chip_shutdown;
	long irq_chip_enable;
	long irq_chip_disable;
	long irq_chip_ack;
	long irq_chip_end;
	long irq_chip_set_affinity;
	long irq_chip_mask;
	long irq_chip_mask_ack;
	long irq_chip_unmask;
	long irq_chip_eoi;
	long irq_chip_retrigger;
	long irq_chip_set_type;
	long irq_chip_set_wake;
	long irq_cpustat_t___softirq_active;
	long irq_cpustat_t___softirq_mask;
	long fdtable_max_fds;
	long fdtable_max_fdset;
	long fdtable_open_fds;
	long fdtable_fd;
	long files_struct_fdt;
	long files_struct_max_fds;
	long files_struct_max_fdset;
	long files_struct_open_fds;
	long files_struct_fd;
	long files_struct_open_fds_init;
	long file_f_dentry;
	long file_f_vfsmnt;
	long file_f_count;
	long file_f_path;
	long path_mnt;
	long path_dentry;
	long fs_struct_root;
	long fs_struct_pwd;
	long fs_struct_rootmnt;
	long fs_struct_pwdmnt;
	long dentry_d_inode;
	long dentry_d_parent;
	long dentry_d_name;
	long dentry_d_covers;
	long dentry_d_iname;
	long qstr_len;
	long qstr_name;
	long inode_i_mode;
	long inode_i_op;
	long inode_i_sb;
	long inode_u;
	long inode_i_flock;
	long inode_i_fop;
	long inode_i_mapping;
	long address_space_nrpages;
	long vfsmount_mnt_next;
	long vfsmount_mnt_devname;
	long vfsmount_mnt_dirname;
	long vfsmount_mnt_sb;
	long vfsmount_mnt_list;
	long vfsmount_mnt_mountpoint;
	long vfsmount_mnt_parent;
	long namespace_root;
	long namespace_list;
	long super_block_s_dirty;
	long super_block_s_type;
	long super_block_s_files;
	long file_system_type_name;
	long nlm_file_f_file;
	long file_lock_fl_owner;
	long nlm_host_h_exportent;
	long svc_client_cl_ident;
	long kmem_cache_s_c_nextp;
	long kmem_cache_s_c_name;
	long kmem_cache_s_c_num;
	long kmem_cache_s_c_org_size;
	long kmem_cache_s_c_flags;
	long kmem_cache_s_c_offset;
	long kmem_cache_s_c_firstp;
	long kmem_cache_s_c_gfporder;
	long kmem_cache_s_c_magic;
	long kmem_cache_s_num;
	long kmem_cache_s_next;
	long kmem_cache_s_name;
	long kmem_cache_s_objsize;
	long kmem_cache_s_flags;
	long kmem_cache_s_gfporder;
	long kmem_cache_s_slabs;
	long kmem_cache_s_slabs_full;
	long kmem_cache_s_slabs_partial;
	long kmem_cache_s_slabs_free;
	long kmem_cache_s_cpudata;
	long kmem_cache_s_c_align;
	long kmem_cache_s_colour_off;
	long cpucache_s_avail;
	long cpucache_s_limit;
	long kmem_cache_s_array;
	long array_cache_avail;
	long array_cache_limit;
	long kmem_cache_s_lists;
	long kmem_list3_slabs_partial;
	long kmem_list3_slabs_full;
	long kmem_list3_slabs_free;
	long kmem_list3_free_objects;
	long kmem_list3_shared;
	long kmem_slab_s_s_nextp;
	long kmem_slab_s_s_freep;
	long kmem_slab_s_s_inuse;
	long kmem_slab_s_s_mem;
	long kmem_slab_s_s_index;
	long kmem_slab_s_s_offset;
	long kmem_slab_s_s_magic;
	long slab_s_list;
	long slab_s_s_mem;
	long slab_s_inuse;
	long slab_s_free;
	long slab_list;
	long slab_s_mem;
	long slab_inuse;
	long slab_free;
	long net_device_next;
	long net_device_name;
	long net_device_type;
	long net_device_addr_len;
	long net_device_ip_ptr;
	long net_device_dev_list;
	long net_dev_base_head;
	long device_next;
	long device_name;
	long device_type;
	long device_ip_ptr;
	long device_addr_len;
	long socket_sk;
	long sock_daddr;
	long sock_rcv_saddr;
	long sock_dport;
	long sock_sport;
	long sock_num;
	long sock_type;
	long sock_family;
	long sock_common_skc_family;
	long sock_sk_type;
	long inet_sock_inet;
	long inet_opt_daddr;
	long inet_opt_rcv_saddr;
	long inet_opt_dport;
	long inet_opt_sport;
	long inet_opt_num;
	long ipv6_pinfo_rcv_saddr;
	long ipv6_pinfo_daddr;
	long timer_list_list;
	long timer_list_next;
	long timer_list_entry;
	long timer_list_expires;
	long timer_list_function;
	long timer_vec_root_vec;
	long timer_vec_vec;
	long tvec_root_s_vec;
	long tvec_s_vec;
	long tvec_t_base_s_tv1;
	long wait_queue_task;
	long wait_queue_next;
	long __wait_queue_task;
	long __wait_queue_head_task_list;
	long __wait_queue_task_list;
	long pglist_data_node_zones;
	long pglist_data_node_mem_map;
	long pglist_data_node_start_paddr;
	long pglist_data_node_start_mapnr;
	long pglist_data_node_size;
	long pglist_data_node_id;
	long pglist_data_node_next;
	long pglist_data_nr_zones;
	long pglist_data_node_start_pfn;
	long pglist_data_pgdat_next;
	long pglist_data_node_present_pages;
	long pglist_data_node_spanned_pages;
	long pglist_data_bdata;
	long page_cache_bucket_chain;
	long zone_struct_free_pages;
	long zone_struct_free_area;
	long zone_struct_zone_pgdat;
	long zone_struct_name;
	long zone_struct_size;
	long zone_struct_memsize;
	long zone_struct_zone_start_pfn;
	long zone_struct_zone_start_paddr;
	long zone_struct_zone_start_mapnr;
	long zone_struct_zone_mem_map;
	long zone_struct_inactive_clean_pages;
	long zone_struct_inactive_clean_list;
	long zone_struct_inactive_dirty_pages;
	long zone_struct_active_pages;
	long zone_struct_pages_min;
	long zone_struct_pages_low;
	long zone_struct_pages_high;
	long zone_free_pages;
	long zone_free_area;
	long zone_zone_pgdat;
	long zone_zone_mem_map;
	long zone_name;
	long zone_spanned_pages;
	long zone_zone_start_pfn;
	long zone_pages_min;
	long zone_pages_low;
	long zone_pages_high;
	long zone_vm_stat;
	long neighbour_next;
	long neighbour_primary_key;
	long neighbour_ha;
	long neighbour_dev;
	long neighbour_nud_state;
	long neigh_table_hash_buckets;
	long neigh_table_key_len;
	long in_device_ifa_list;
	long in_ifaddr_ifa_next;
	long in_ifaddr_ifa_address;
	long pci_dev_global_list;
	long pci_dev_next;
	long pci_dev_bus;
	long pci_dev_devfn;
	long pci_dev_class;
	long pci_dev_device;
	long pci_dev_vendor;
	long pci_bus_number;
	long resource_entry_t_from;
	long resource_entry_t_num;
	long resource_entry_t_name; 
	long resource_entry_t_next;
	long resource_name;
	long resource_start;
	long resource_end;
	long resource_sibling;
	long resource_child;
	long runqueue_curr;
	long runqueue_idle;
	long runqueue_active;
	long runqueue_expired;
	long runqueue_arrays;
	long runqueue_cpu;
	long cpu_s_idle;
	long cpu_s_curr;
	long prio_array_nr_active;
	long prio_array_queue;
	long user_regs_struct_ebp;
	long user_regs_struct_esp;
	long user_regs_struct_rip;
	long user_regs_struct_cs;
	long user_regs_struct_eflags;
	long user_regs_struct_rsp;
	long user_regs_struct_ss;
	long e820map_nr_map;
	long e820entry_addr;	
	long e820entry_size;	
	long e820entry_type;	
	long char_device_struct_next;
	long char_device_struct_name;
	long char_device_struct_fops;
	long char_device_struct_major;
	long gendisk_major;
	long gendisk_disk_name;
	long gendisk_fops;
	long blk_major_name_next;
	long blk_major_name_major;
	long blk_major_name_name;
	long radix_tree_root_height;
	long radix_tree_root_rnode;
	long x8664_pda_pcurrent;
	long x8664_pda_data_offset;
	long x8664_pda_kernelstack;
	long x8664_pda_irqrsp;
	long x8664_pda_irqstackptr;
	long x8664_pda_level4_pgt;
	long x8664_pda_cpunumber;
	long x8664_pda_me;
	long tss_struct_ist;
	long mem_section_section_mem_map;
	long vcpu_guest_context_user_regs;
	long cpu_user_regs_eip;
	long cpu_user_regs_esp;
	long cpu_user_regs_rip;
	long cpu_user_regs_rsp;
	long unwind_table_core;
	long unwind_table_init;
	long unwind_table_address;
	long unwind_table_size;
	long unwind_table_link;
	long unwind_table_name;
	long rq_cfs;
	long rq_rt;
	long rq_nr_running;
	long cfs_rq_rb_leftmost;
	long cfs_rq_nr_running;
	long cfs_rq_tasks_timeline;
	long task_struct_se;
	long sched_entity_run_node;
	long rt_rq_active;
	long kmem_cache_size;
	long kmem_cache_objsize;
	long kmem_cache_offset;
	long kmem_cache_order;
	long kmem_cache_local_node;
	long kmem_cache_objects;
	long kmem_cache_inuse;
	long kmem_cache_align;
	long kmem_cache_name;
	long kmem_cache_list;
	long kmem_cache_node;
	long kmem_cache_cpu_slab;
	long page_inuse;
/*	long page_offset;  use "old" page->offset */
	long page_slab;
	long page_first_page;
	long page_freelist;
	long kmem_cache_node_nr_partial;
	long kmem_cache_node_nr_slabs;
	long kmem_cache_node_partial;
	long kmem_cache_node_full;
	long pid_numbers;
	long upid_nr;
	long upid_ns;
	long upid_pid_chain;
	long pid_tasks;
	long kmem_cache_cpu_freelist;
	long kmem_cache_cpu_page;
	long kmem_cache_cpu_node;
	long kmem_cache_flags;
	long zone_nr_active;
	long zone_nr_inactive;
	long zone_all_unreclaimable;
	long zone_present_pages;
	long zone_flags;
	long zone_pages_scanned;
	long pcpu_info_vcpu;
	long pcpu_info_idle;
	long vcpu_struct_rq;
	long task_struct_sched_info;
	long sched_info_last_arrival;
	long page_objects;
	long kmem_cache_oo;
	long char_device_struct_cdev;
	long char_device_struct_baseminor;
	long cdev_ops;
	long probe_next;
	long probe_dev;
	long probe_data;
	long kobj_map_probes;
	long task_struct_prio;
	long zone_watermark;
	long module_sect_attrs;
	long module_sect_attrs_attrs;
	long module_sect_attrs_nsections;
	long module_sect_attr_mattr;
	long module_sect_attr_name;
	long module_sect_attr_address;
	long module_attribute_attr;
	long attribute_owner;
	long module_sect_attr_attr;
	long module_sections_attrs;
	long swap_info_struct_inuse_pages;
	long s390_lowcore_psw_save_area;
	long mm_struct_rss_stat;
	long mm_rss_stat_count;
	long module_module_init;
	long module_init_text_size;
	long cpu_context_save_fp;
	long cpu_context_save_sp;
	long cpu_context_save_pc;
	long elf_prstatus_pr_pid;
	long elf_prstatus_pr_reg;
	long irq_desc_t_name;
	long thread_info_cpu_context;
	long unwind_table_list;
	long unwind_table_start;
	long unwind_table_stop;
	long unwind_table_begin_addr;
	long unwind_table_end_addr;
	long unwind_idx_addr;
	long unwind_idx_insn;
	long signal_struct_nr_threads;
	long module_init_size;
	long module_percpu;
	long radix_tree_node_slots;
	long s390_stack_frame_back_chain;
	long s390_stack_frame_r14;
	long user_regs_struct_eip;
	long user_regs_struct_rax;
	long user_regs_struct_eax;
	long user_regs_struct_rbx;
	long user_regs_struct_ebx;
	long user_regs_struct_rcx;
	long user_regs_struct_ecx;
	long user_regs_struct_rdx;
	long user_regs_struct_edx;
	long user_regs_struct_rsi;
	long user_regs_struct_esi;
	long user_regs_struct_rdi;
	long user_regs_struct_edi;
	long user_regs_struct_ds;
	long user_regs_struct_es;
	long user_regs_struct_fs;
	long user_regs_struct_gs;
	long user_regs_struct_rbp;
	long user_regs_struct_r8;
	long user_regs_struct_r9;
	long user_regs_struct_r10;
	long user_regs_struct_r11;
	long user_regs_struct_r12;
	long user_regs_struct_r13;
	long user_regs_struct_r14;
	long user_regs_struct_r15;
	long sched_entity_cfs_rq;
	long sched_entity_my_q;
	long sched_entity_on_rq;
	long task_struct_on_rq;
	long cfs_rq_curr;
	long irq_desc_t_irq_data;
	long irq_desc_t_kstat_irqs;
	long irq_desc_t_affinity;
	long irq_data_chip;
	long irq_data_affinity;
	long kernel_stat_irqs;
	long socket_alloc_vfs_inode;
	long class_devices;
	long class_p;
	long class_private_devices;
	long device_knode_class;
	long device_node;
	long gendisk_dev;
	long gendisk_kobj;
	long gendisk_part0;
	long gendisk_queue;
	long hd_struct_dev;
	long klist_k_list;
	long klist_node_n_klist;
	long klist_node_n_node;
	long kobject_entry;
	long kset_list;
	long request_list_count;
	long request_queue_in_flight;
	long request_queue_rq;
	long subsys_private_klist_devices;
	long subsystem_kset;
	long mount_mnt_parent;
	long mount_mnt_mountpoint;
	long mount_mnt_list;
	long mount_mnt_devname;
	long mount_mnt;
	long task_struct_exit_state;
	long timekeeper_xtime;
	long file_f_op;
	long file_private_data;
	long hstate_order;
	long hugetlbfs_sb_info_hstate;
	long idr_layer_ary;
	long idr_layer_layer;
	long idr_layers;
	long idr_top;
	long ipc_id_ary_p;
	long ipc_ids_entries;
	long ipc_ids_max_id;
	long ipc_ids_ipcs_idr;
	long ipc_ids_in_use;
	long ipc_namespace_ids;
	long kern_ipc_perm_deleted;
	long kern_ipc_perm_key;
	long kern_ipc_perm_mode;
	long kern_ipc_perm_uid;
	long kern_ipc_perm_id;
	long kern_ipc_perm_seq;
	long nsproxy_ipc_ns;
	long shmem_inode_info_swapped;
	long shmem_inode_info_vfs_inode;
	long shm_file_data_file;
	long shmid_kernel_shm_file;
	long shmid_kernel_shm_nattch;
	long shmid_kernel_shm_perm;
	long shmid_kernel_shm_segsz;
	long shmid_kernel_id;
	long sem_array_sem_perm;
	long sem_array_sem_id;
	long sem_array_sem_nsems;
	long msg_queue_q_perm;
	long msg_queue_q_id;
	long msg_queue_q_cbytes;
	long msg_queue_q_qnum;
	long super_block_s_fs_info;
	long rq_timestamp;
	long radix_tree_node_height;
	long rb_root_rb_node;
	long rb_node_rb_left;
	long rb_node_rb_right;
	long rt_prio_array_queue;
	long task_struct_rt;
	long sched_rt_entity_run_list;
	long log_ts_nsec;
	long log_len;
	long log_text_len;
	long log_dict_len;
	long log_level;
	long log_flags_level;
	long timekeeper_xtime_sec;
	long neigh_table_hash_mask;
	long sched_rt_entity_my_q;
	long neigh_table_hash_shift;
	long neigh_table_nht_ptr;
	long task_group_parent;
	long task_group_css;
	long cgroup_subsys_state_cgroup;
	long cgroup_dentry;
	long task_group_rt_rq;
	long rt_rq_tg;
	long task_group_cfs_rq;
	long cfs_rq_tg;
	long task_group_siblings;
	long task_group_children;
	long task_group_cfs_bandwidth;
	long cfs_rq_throttled;
	long task_group_rt_bandwidth;
	long rt_rq_rt_throttled;
	long rt_rq_highest_prio;
	long rt_rq_rt_nr_running;
	long vmap_area_va_start;
	long vmap_area_va_end;
	long vmap_area_list;
	long vmap_area_flags;
	long vmap_area_vm;
	long hrtimer_cpu_base_clock_base;
	long hrtimer_clock_base_offset;
	long hrtimer_clock_base_active;
	long hrtimer_clock_base_first;
	long hrtimer_clock_base_get_time;
	long hrtimer_base_first;
	long hrtimer_base_pending;
	long hrtimer_base_get_time;
	long hrtimer_node;
	long hrtimer_list;
	long hrtimer_softexpires;
	long hrtimer_expires;
	long hrtimer_function;
	long timerqueue_head_next;
	long timerqueue_node_expires;
	long timerqueue_node_node;
	long ktime_t_tv64;
	long ktime_t_sec;
	long ktime_t_nsec;
	long module_taints;
	long module_gpgsig_ok;
	long module_license_gplok;
	long tnt_bit;
	long tnt_true;
	long tnt_false;
	long task_struct_thread_context_fp;
	long task_struct_thread_context_sp;
	long task_struct_thread_context_pc;
	long page_slab_page;
	long trace_print_flags_mask;
	long trace_print_flags_name;
	long task_struct_rss_stat;
	long task_rss_stat_count;
	long page_s_mem;
	long page_active;
	long hstate_nr_huge_pages;
	long hstate_free_huge_pages;
	long hstate_name;
	long cgroup_kn;
	long kernfs_node_name;
	long kernfs_node_parent;
	long kmem_cache_cpu_partial;
	long kmem_cache_cpu_cache;
	long nsproxy_net_ns;
	long atomic_t_counter;
	long percpu_counter_count;
	long mm_struct_mm_count;
	long task_struct_thread_reg29;
	long task_struct_thread_reg31;
	long pt_regs_regs;
	long pt_regs_cp0_badvaddr;
	long address_space_page_tree;
	long page_compound_head;
	long irq_desc_irq_data;
	long kmem_cache_node_total_objects;
	long timer_base_vectors;
	long request_queue_mq_ops;
	long request_queue_queue_ctx;
	long blk_mq_ctx_rq_dispatched;
	long blk_mq_ctx_rq_completed;
	long task_struct_stack;
	long tnt_mod;
	long radix_tree_node_shift;
	long kmem_cache_red_left_pad;
	long inactive_task_frame_ret_addr;
	long sk_buff_head_next;
	long sk_buff_head_qlen;
	long sk_buff_next;
	long sk_buff_len;
	long sk_buff_data;
	long nlmsghdr_nlmsg_type;
	long module_arch;
	long mod_arch_specific_num_orcs;
	long mod_arch_specific_orc_unwind_ip;
	long mod_arch_specific_orc_unwind;
	long task_struct_policy;
	long kmem_cache_random;
	long pid_namespace_idr;
	long idr_idr_rt;
	long bpf_prog_aux;
	long bpf_prog_type;
	long bpf_prog_tag;
	long bpf_prog_jited_len;
	long bpf_prog_bpf_func;
	long bpf_prog_len;
	long bpf_prog_insnsi;
	long bpf_prog_pages;
	long bpf_map_map_type;
	long bpf_map_map_flags;
	long bpf_map_pages;
	long bpf_map_key_size;
	long bpf_map_value_size;
	long bpf_map_max_entries;
	long bpf_map_user;
	long bpf_map_name;
	long bpf_prog_aux_used_map_cnt;
	long bpf_prog_aux_used_maps;
	long bpf_prog_aux_load_time;
	long bpf_prog_aux_user;
	long user_struct_uid;
	long idr_cur;
	long kmem_cache_memcg_params;
	long memcg_cache_params___root_caches_node;
	long memcg_cache_params_children;
	long memcg_cache_params_children_node;
	long task_struct_pid_links;
	long kernel_symbol_value;
	long pci_dev_dev;
	long pci_dev_hdr_type;
	long pci_dev_pcie_flags_reg;
	long pci_bus_node;
	long pci_bus_devices;
	long pci_bus_dev;
	long pci_bus_children;
	long pci_bus_parent;
	long pci_bus_self;
	long device_kobj;
	long kobject_name;
	long memory_block_dev;
	long memory_block_start_section_nr;
	long memory_block_end_section_nr;
	long memory_block_state;
	long memory_block_nid;
	long mem_section_pageblock_flags;
	long bus_type_p;
	long device_private_device;
	long device_private_knode_bus;
	long xarray_xa_head;
	long xa_node_slots;
	long xa_node_shift;
	long hd_struct_dkstats;
	long disk_stats_in_flight;
	long cpu_context_save_r7;
	long dentry_d_sb;
	long device_private_knode_class;
	long timerqueue_head_rb_root;
	long rb_root_cached_rb_leftmost;
	long bpf_map_memory;
	long bpf_map_memory_pages;
	long bpf_map_memory_user;
	long bpf_prog_aux_name;
	long page_private;
	long swap_info_struct_bdev;
	long zram_mempoll;
	long zram_compressor;
	long zram_table_flag;
	long zspoll_size_class;
	long size_class_size;
	long gendisk_private_data;
	long zram_table_entry;
	long module_core_size_rw;
	long module_core_size_rx;
	long module_init_size_rw;
	long module_init_size_rx;
	long module_module_core_rw;
	long module_module_core_rx;
	long module_module_init_rw;
	long module_module_init_rx;
	long super_block_s_inodes;
	long inode_i_sb_list;
	long irq_common_data_affinity;
	long irq_desc_irq_common_data;
	long uts_namespace_name;
	long printk_info_seq;
	long printk_info_ts_nsec;
	long printk_info_text_len;
	long printk_info_level;
	long printk_info_caller_id;
	long printk_info_dev_info;
	long dev_printk_info_subsystem;
	long dev_printk_info_device;
	long prb_desc_ring;
	long prb_text_data_ring;
	long prb_desc_ring_count_bits;
	long prb_desc_ring_descs;
	long prb_desc_ring_infos;
	long prb_desc_ring_head_id;
	long prb_desc_ring_tail_id;
	long prb_desc_state_var;
	long prb_desc_text_blk_lpos;
	long prb_data_blk_lpos_begin;
	long prb_data_blk_lpos_next;
	long prb_data_ring_size_bits;
	long prb_data_ring_data;
	long atomic_long_t_counter;
	long block_device_bd_device;
	long block_device_bd_stats;
};

extern struct offset_table offset_table;

struct size_table {         /* stash of commonly-used sizes */
	long page;
	long free_area_struct;
	long zone_struct;
	long free_area;
	long zone;
	long kmem_slab_s;
	long kmem_cache_s;
	long kmem_bufctl_t;
	long slab_s;
	long slab;
	long cpucache_s;
	long array_cache;
	long swap_info_struct;
	long mm_struct;
	long vm_area_struct;
	long pglist_data;
	long page_cache_bucket;
	long pt_regs;
	long task_struct;
	long thread_info;
	long softirq_state;
	long desc_struct;
	long umode_t;
	long dentry;
	long files_struct;
	long fdtable;
	long fs_struct;
	long file;
	long inode;
	long vfsmount;
	long super_block;
	long irqdesc;
	long module;
	long list_head;
	long hlist_node;
	long hlist_head;
	long irq_cpustat_t;
	long cpuinfo_x86;
	long cpuinfo_ia64;
	long timer_list;
	long timer_vec_root;
	long timer_vec;
	long tvec_root_s;
	long tvec_s;
	long tvec_t_base_s;
	long wait_queue;
	long __wait_queue;
	long device;
	long net_device;
	long sock;
	long signal_struct;
	long sigpending_signal;
	long signal_queue;
	long sighand_struct;
	long sigqueue;
	long k_sigaction;
	long resource_entry_t;
	long resource;
	long runqueue;
	long irq_desc_t;
	long task_union;
	long thread_union;
	long prio_array;
	long user_regs_struct;
	long switch_stack;
	long vm_area_struct_vm_flags;
	long e820map;
	long e820entry;
	long cpu_s;
	long pgd_t;
	long kallsyms_header;
	long kallsyms_symbol;
	long kallsyms_section;
	long irq_ctx;
	long block_device;
	long blk_major_name;
	long gendisk;
	long address_space;
	long char_device_struct;
	long inet_sock;
	long in6_addr;
	long socket;
	long spinlock_t;
	long radix_tree_root;
	long radix_tree_node;
	long x8664_pda;
	long ppc64_paca;
	long gate_struct;
	long tss_struct;
	long task_struct_start_time;
	long cputime_t;
	long mem_section;
	long pid_link;
	long unwind_table;
	long rlimit;
	long kmem_cache;
	long kmem_cache_node;
	long upid;
	long kmem_cache_cpu;
	long cfs_rq;
	long pcpu_info;
	long vcpu_struct;
	long cdev;
	long probe;
	long kobj_map;
	long page_flags;
	long module_sect_attr;
	long task_struct_utime;
	long task_struct_stime;
	long cpu_context_save;
	long elf_prstatus;
	long note_buf;
	long unwind_idx;
	long softirq_action;
	long irq_data;
	long s390_stack_frame;
	long percpu_data;
	long sched_entity;
	long kernel_stat;
	long subsystem;
	long class_private;
	long rq_in_flight;
	long class_private_devices;
	long mount;
	long hstate;
	long ipc_ids;
	long shmid_kernel;
	long sem_array;
	long msg_queue;
	long log;
	long log_level;
	long rt_rq;
	long task_group;
	long vmap_area;
	long hrtimer_clock_base;
	long hrtimer_base;
	long tnt;
	long trace_print_flags;
	long task_struct_flags;
	long timer_base;
	long taint_flag;
	long nlmsghdr;
	long nlmsghdr_nlmsg_type;
	long sk_buff_head_qlen;
	long sk_buff_len;
	long orc_entry;
	long task_struct_policy;
	long pid;
	long bpf_prog;
	long bpf_prog_aux;
	long bpf_map;
	long bpf_insn;
	long xarray;
	long xa_node;
	long zram_table_entry;
	long irq_common_data;
	long printk_info;
	long printk_ringbuffer;
	long prb_desc;
};

#define MAX_MACHDEP_ARGS 5  /* for --machdep/-m machine-specific args */

struct mach_table {
	unsigned long flags;
	unsigned long kvbase;
	unsigned long identity_map_base;
	unsigned int pagesize;
	unsigned int pageshift;
	unsigned long long pagemask;
	unsigned long pageoffset;
	unsigned long stacksize;
	unsigned int hz;
	unsigned long mhz;
	int bits;
	int nr_irqs;
	uint64_t memsize;
	// int (*eframe_search)(struct bt_info *);
	// void (*back_trace)(struct bt_info *);
	unsigned long (*processor_speed)(void);
	unsigned long (*get_task_pgd)(unsigned long);
	void (*dump_irq)(int);
	// void (*get_stack_frame)(struct bt_info *, unsigned long *, unsigned long *);
	unsigned long (*get_stackbase)(unsigned long);
	unsigned long (*get_stacktop)(unsigned long);
	int (*translate_pte)(unsigned long, void *, unsigned long long);
	uint64_t (*memory_size)(void);
	unsigned long (*vmalloc_start)(void);
	int (*is_task_addr)(unsigned long);
	int (*verify_symbol)(const char *, unsigned long, char);
	int (*dis_filter)(unsigned long, char *, unsigned int);
	int (*get_smp_cpus)(void);
	int (*is_kvaddr)(unsigned long);
	int (*is_uvaddr)(unsigned long, struct task_context *);
	int (*verify_paddr)(uint64_t);
	void (*cmd_mach)(void);
	void (*init_kernel_pgd)(void);
	// struct syment *(*value_to_symbol)(unsigned long, unsigned long *);
	struct line_number_hook {
		char *func;
		char **file;
	} *line_number_hooks;
	unsigned long last_pgd_read;
	unsigned long last_pud_read;
	unsigned long last_pmd_read;
	unsigned long last_ptbl_read;
	char *pgd;
	char *pud;
	char *pmd;	
	char *ptbl;
	int ptrs_per_pgd;
	char *cmdline_args[MAX_MACHDEP_ARGS];
	struct arch_machine_descriptor *mdesp;
	unsigned long section_size_bits;
	unsigned long max_physmem_bits;
	unsigned long sections_per_root;
	// int (*xendump_p2m_create)(struct xendump_data *);
	//unsigned long (*xendump_panic_task)(struct xendump_data *);
	//void (*get_xendump_regs)(struct xendump_data *, struct bt_info *, unsigned long *, unsigned long *);
	void (*clear_machdep_cache)(void);
	// int (*xen_kdump_p2m_create)(struct xen_kdump_data *);
	int (*in_alternate_stack)(int, unsigned long);
	void (*dumpfile_init)(int, void *);
	void (*process_elf_notes)(void *, unsigned long);
	// int (*get_kvaddr_ranges)(struct vaddr_range *);
	int (*verify_line_number)(unsigned long, unsigned long, unsigned long);
	void (*get_irq_affinity)(int);
	void (*show_interrupts)(int, unsigned long *);
	int (*is_page_ptr)(unsigned long, physaddr_t *);
};
extern struct mach_table kcoreinfo_data;
extern struct mach_table *kcoreinfo;

struct datatype_member {        /* minimal definition of a structure/union */
	char *name;             /* and possibly a member within it */
	char *member;
	unsigned long type;
	long size;
	long member_offset;
	long member_size;
	int member_typecode;
	unsigned long flags;
	char *tagname;         /* tagname and value for enums */
	long value;
	unsigned long vaddr;
};

#endif
