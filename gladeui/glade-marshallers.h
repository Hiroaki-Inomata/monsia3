
#ifndef __glade_marshal_MARSHAL_H__
#define __glade_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:POINTER,POINTER (./glade-marshallers.list:1) */
extern void glade_marshal_VOID__POINTER_POINTER (GClosure     *closure,
                                                 GValue       *return_value,
                                                 guint         n_param_values,
                                                 const GValue *param_values,
                                                 gpointer      invocation_hint,
                                                 gpointer      marshal_data);

/* VOID:STRING,ULONG,UINT,STRING (./glade-marshallers.list:2) */
extern void glade_marshal_VOID__STRING_ULONG_UINT_STRING (GClosure     *closure,
                                                          GValue       *return_value,
                                                          guint         n_param_values,
                                                          const GValue *param_values,
                                                          gpointer      invocation_hint,
                                                          gpointer      marshal_data);

/* VOID:OBJECT (./glade-marshallers.list:3) */
#define glade_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID:OBJECT,BOOLEAN (./glade-marshallers.list:4) */
extern void glade_marshal_VOID__OBJECT_BOOLEAN (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint,
                                                gpointer      marshal_data);

/* VOID:STRING,STRING,STRING (./glade-marshallers.list:5) */
extern void glade_marshal_VOID__STRING_STRING_STRING (GClosure     *closure,
                                                      GValue       *return_value,
                                                      guint         n_param_values,
                                                      const GValue *param_values,
                                                      gpointer      invocation_hint,
                                                      gpointer      marshal_data);

/* OBJECT:POINTER (./glade-marshallers.list:6) */
extern void glade_marshal_OBJECT__POINTER (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);

/* OBJECT:OBJECT,UINT (./glade-marshallers.list:7) */
extern void glade_marshal_OBJECT__OBJECT_UINT (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

/* BOOLEAN:STRING (./glade-marshallers.list:8) */
extern void glade_marshal_BOOLEAN__STRING (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);

/* BOOLEAN:BOXED (./glade-marshallers.list:9) */
extern void glade_marshal_BOOLEAN__BOXED (GClosure     *closure,
                                          GValue       *return_value,
                                          guint         n_param_values,
                                          const GValue *param_values,
                                          gpointer      invocation_hint,
                                          gpointer      marshal_data);

/* BOOLEAN:OBJECT (./glade-marshallers.list:10) */
extern void glade_marshal_BOOLEAN__OBJECT (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);

/* BOOLEAN:OBJECT,BOXED (./glade-marshallers.list:11) */
extern void glade_marshal_BOOLEAN__OBJECT_BOXED (GClosure     *closure,
                                                 GValue       *return_value,
                                                 guint         n_param_values,
                                                 const GValue *param_values,
                                                 gpointer      invocation_hint,
                                                 gpointer      marshal_data);

/* BOOLEAN:OBJECT,POINTER (./glade-marshallers.list:12) */
extern void glade_marshal_BOOLEAN__OBJECT_POINTER (GClosure     *closure,
                                                   GValue       *return_value,
                                                   guint         n_param_values,
                                                   const GValue *param_values,
                                                   gpointer      invocation_hint,
                                                   gpointer      marshal_data);

/* BOOLEAN:OBJECT,BOOLEAN (./glade-marshallers.list:13) */
extern void glade_marshal_BOOLEAN__OBJECT_BOOLEAN (GClosure     *closure,
                                                   GValue       *return_value,
                                                   guint         n_param_values,
                                                   const GValue *param_values,
                                                   gpointer      invocation_hint,
                                                   gpointer      marshal_data);

/* BOOLEAN:OBJECT,UINT (./glade-marshallers.list:14) */
extern void glade_marshal_BOOLEAN__OBJECT_UINT (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint,
                                                gpointer      marshal_data);

/* BOOLEAN:OBJECT,OBJECT (./glade-marshallers.list:15) */
extern void glade_marshal_BOOLEAN__OBJECT_OBJECT (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data);

/* BOOLEAN:OBJECT,STRING (./glade-marshallers.list:16) */
extern void glade_marshal_BOOLEAN__OBJECT_STRING (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data);

/* STRING:OBJECT (./glade-marshallers.list:17) */
extern void glade_marshal_STRING__OBJECT (GClosure     *closure,
                                          GValue       *return_value,
                                          guint         n_param_values,
                                          const GValue *param_values,
                                          gpointer      invocation_hint,
                                          gpointer      marshal_data);

G_END_DECLS

#endif /* __glade_marshal_MARSHAL_H__ */

