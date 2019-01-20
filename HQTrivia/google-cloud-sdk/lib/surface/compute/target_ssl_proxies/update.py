# Copyright 2014 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""Command for updating target SSL proxies."""

from googlecloudsdk.api_lib.compute import base_classes
from googlecloudsdk.api_lib.compute import target_proxies_utils
from googlecloudsdk.api_lib.compute import utils
from googlecloudsdk.calliope import base
from googlecloudsdk.calliope import exceptions
from googlecloudsdk.command_lib.compute.backend_services import (
    flags as backend_service_flags)
from googlecloudsdk.command_lib.compute.ssl_certificates import (
    flags as ssl_certificates_flags)
from googlecloudsdk.command_lib.compute.ssl_policies import (flags as
                                                             ssl_policies_flags)
from googlecloudsdk.command_lib.compute.target_ssl_proxies import flags
from googlecloudsdk.core import log


@base.ReleaseTracks(base.ReleaseTrack.GA)
class UpdateGA(base.SilentCommand):
  """Update a target SSL proxy.

  *{command}* is used to replace the SSL certificate, backend service or proxy
  header of existing target SSL proxies. A target SSL proxy is referenced by
  one or more forwarding rules which define which packets the proxy is
  responsible for routing. The target SSL proxy in turn points to a backend
  service which will handle the requests. The target SSL proxy also points to
  at most 10 SSL certificates used for server-side authentication.
  """

  BACKEND_SERVICE_ARG = None
  SSL_CERTIFICATE_ARG = None
  SSL_CERTIFICATES_ARG = None
  TARGET_SSL_PROXY_ARG = None

  @classmethod
  def Args(cls, parser):
    target_proxies_utils.AddProxyHeaderRelatedUpdateArgs(parser)

    cls.BACKEND_SERVICE_ARG = (
        backend_service_flags.BackendServiceArgumentForTargetSslProxy(
            required=False))
    cls.BACKEND_SERVICE_ARG.AddArgument(parser)
    cls.TARGET_SSL_PROXY_ARG = flags.TargetSslProxyArgument()
    cls.TARGET_SSL_PROXY_ARG.AddArgument(parser, operation_type='update')

    certs = parser.add_mutually_exclusive_group()
    cls.SSL_CERTIFICATE_ARG = (
        ssl_certificates_flags.SslCertificateArgumentForOtherResource(
            'target SSL proxy', required=False))
    cls.SSL_CERTIFICATE_ARG.AddArgument(parser, mutex_group=certs)
    cls.SSL_CERTIFICATES_ARG = (
        ssl_certificates_flags.SslCertificatesArgumentForOtherResource(
            'target SSL proxy', required=False))
    cls.SSL_CERTIFICATES_ARG.AddArgument(
        parser, mutex_group=certs, cust_metavar='SSL_CERTIFICATE')

  def _GetSslCertificatesList(self, args, holder):
    if args.ssl_certificate:
      log.warn(
          'The --ssl-certificate flag is deprecated and will be removed soon. '
          'Use equivalent --ssl-certificates %s flag.', args.ssl_certificate)
      return [
          self.SSL_CERTIFICATE_ARG.ResolveAsResource(args, holder.resources)
      ]

    if args.ssl_certificates:
      return self.SSL_CERTIFICATES_ARG.ResolveAsResource(args, holder.resources)

    return []

  def _SendRequests(self,
                    args,
                    ssl_cert_refs,
                    ssl_policy=None,
                    clear_ssl_policy=False):
    holder = base_classes.ComputeApiHolder(self.ReleaseTrack())
    requests = []
    target_ssl_proxy_ref = self.TARGET_SSL_PROXY_ARG.ResolveAsResource(
        args, holder.resources)

    client = holder.client.apitools_client
    messages = holder.client.messages

    if ssl_cert_refs:
      requests.append(
          (client.targetSslProxies, 'SetSslCertificates',
           messages.ComputeTargetSslProxiesSetSslCertificatesRequest(
               project=target_ssl_proxy_ref.project,
               targetSslProxy=target_ssl_proxy_ref.Name(),
               targetSslProxiesSetSslCertificatesRequest=(
                   messages.TargetSslProxiesSetSslCertificatesRequest(
                       sslCertificates=[
                           ref.SelfLink() for ref in ssl_cert_refs
                       ])))))

    if args.backend_service:
      backend_service_ref = self.BACKEND_SERVICE_ARG.ResolveAsResource(
          args, holder.resources)
      requests.append(
          (client.targetSslProxies, 'SetBackendService',
           messages.ComputeTargetSslProxiesSetBackendServiceRequest(
               project=target_ssl_proxy_ref.project,
               targetSslProxy=target_ssl_proxy_ref.Name(),
               targetSslProxiesSetBackendServiceRequest=(
                   messages.TargetSslProxiesSetBackendServiceRequest(
                       service=backend_service_ref.SelfLink())))))

    if args.proxy_header:
      proxy_header = (messages.TargetSslProxiesSetProxyHeaderRequest.
                      ProxyHeaderValueValuesEnum(args.proxy_header))
      requests.append((client.targetSslProxies, 'SetProxyHeader',
                       messages.ComputeTargetSslProxiesSetProxyHeaderRequest(
                           project=target_ssl_proxy_ref.project,
                           targetSslProxy=target_ssl_proxy_ref.Name(),
                           targetSslProxiesSetProxyHeaderRequest=(
                               messages.TargetSslProxiesSetProxyHeaderRequest(
                                   proxyHeader=proxy_header)))))

    if ssl_policy or clear_ssl_policy:
      requests.append((client.targetSslProxies, 'SetSslPolicy',
                       messages.ComputeTargetSslProxiesSetSslPolicyRequest(
                           project=target_ssl_proxy_ref.project,
                           targetSslProxy=target_ssl_proxy_ref.Name(),
                           sslPolicyReference=ssl_policy)))

    errors = []
    resources = holder.client.MakeRequests(requests, errors)

    if errors:
      utils.RaiseToolException(errors)
    return resources

  def Run(self, args):
    if not (args.ssl_certificate or args.ssl_certificates or
            args.proxy_header or args.backend_service):
      raise exceptions.ToolException(
          'You must specify at least one of [--ssl-certificates], '
          '[--backend-service] or [--proxy-header].')

    holder = base_classes.ComputeApiHolder(self.ReleaseTrack())
    ssl_certificate_refs = self._GetSslCertificatesList(args, holder)
    return self._SendRequests(args, ssl_certificate_refs)


@base.ReleaseTracks(base.ReleaseTrack.ALPHA, base.ReleaseTrack.BETA)
class UpdateAlphaBeta(UpdateGA):
  """Update a target SSL proxy.

  *{command}* is used to replace the SSL certificate, backend service, proxy
  header or SSL policy of existing target SSL proxies. A target SSL proxy is
  referenced by one or more forwarding rules which define which packets the
  proxy is responsible for routing. The target SSL proxy in turn points to a
  backend service which will handle the requests. The target SSL proxy also
  points to at most 10 SSL certificates used for server-side authentication.
  The target SSL proxy can be associated with at most one SSL policy.
  """

  SSL_POLICY_ARG = None

  @classmethod
  def Args(cls, parser):
    super(UpdateAlphaBeta, cls).Args(parser)
    group = parser.add_mutually_exclusive_group()
    cls.SSL_POLICY_ARG = (
        ssl_policies_flags.GetSslPolicyArgumentForOtherResource(
            'SSL', required=False))
    cls.SSL_POLICY_ARG.AddArgument(group)
    ssl_policies_flags.GetClearSslPolicyArgumentForOtherResource(
        'SSL', required=False).AddToParser(group)

  def _CheckMissingArgument(self, args):
    if not sum(
        args.IsSpecified(arg)
        for arg in [
            'ssl_certificates', 'ssl_certificate', 'proxy_header',
            'backend_service', 'ssl_policy', 'clear_ssl_policy'
        ]):
      raise exceptions.ToolException(
          'You must specify at least one of [--ssl-certificates], '
          '[--backend-service], [--proxy-header], [--ssl-policy] or '
          '[--clear-ssl-policy].')

  def Run(self, args):
    self._CheckMissingArgument(args)

    holder = base_classes.ComputeApiHolder(self.ReleaseTrack())
    messages = holder.client.messages
    ssl_certificate_refs = self._GetSslCertificatesList(args, holder)

    ssl_policy = messages.SslPolicyReference(
        sslPolicy=self.SSL_POLICY_ARG.ResolveAsResource(args, holder.resources)
        .SelfLink()) if args.IsSpecified('ssl_policy') else None

    return self._SendRequests(
        args,
        ssl_certificate_refs,
        ssl_policy=ssl_policy,
        clear_ssl_policy=args.clear_ssl_policy)
